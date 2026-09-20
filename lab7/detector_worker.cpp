#include "detector_worker.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <thread>

#include <QCoreApplication>
#include <QTimer>
#include <opencv2/imgproc.hpp>

#include "qt/qt_interop.h"

namespace {

QString findPath(const QString &relativePath)
{
    const QStringList candidates{
        relativePath,
        QCoreApplication::applicationDirPath() + "/../" + relativePath,
        QCoreApplication::applicationDirPath() + "/../../" + relativePath};
    for (const QString &candidate : candidates)
        if (std::filesystem::is_regular_file(candidate.toStdString()))
            return candidate;
    return QString();
}

}

DetectorWorker::DetectorWorker(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(33);
    connect(m_timer, &QTimer::timeout, this, [this] {
        QImage image;
        {
            std::lock_guard<std::mutex> lock(m_imageMutex);
            image = std::move(m_latest);
        }
        if (!image.isNull())
            emit frameChanged(image);
    });
}

DetectorWorker::~DetectorWorker()
{
    stop();
}

void DetectorWorker::start()
{
    if (running())
        return;
    const QString onnx = findPath("yunet_trt_cpp/models/face_detection_yunet_2023mar.onnx");
    const QString engine = findPath("yunet_trt_cpp/models/yunet_fp16.engine");
    if (onnx.isEmpty() || engine.isEmpty()) {
        emit errorOccurred(tr("找不到 YuNet ONNX 或 TensorRT engine"));
        return;
    }
    try {
        m_detector = std::make_unique<YuNetTRT>();
        if (!m_detector->init(onnx.toStdString(), engine.toStdString(), true, false)) {
            emit errorOccurred(tr("TensorRT YuNet 初始化失败"));
            m_detector.reset();
            return;
        }
    } catch (const std::exception &error) {
        emit errorOccurred(QString::fromLocal8Bit(error.what()));
        m_detector.reset();
        return;
    }
    if (!m_detector) {
        emit errorOccurred(tr("YuNet CPU 检测器创建失败"));
        return;
    }
    if (!m_source.open()) {
        emit errorOccurred(m_source.lastError());
        return;
    }
    m_stopRequested.store(false);
    m_running.store(true);
    m_timer->start();
    m_thread = std::thread(&DetectorWorker::loop, this);
    emit statusChanged(tr("YuNet TensorRT 检测运行中，%1").arg(m_source.description()));
}

void DetectorWorker::stop()
{
    m_stopRequested.store(true);
    if (m_thread.joinable())
        m_thread.join();
    m_source.release();
    m_detector.reset();
    m_running.store(false);
    if (m_timer)
        m_timer->stop();
}

void DetectorWorker::loop()
{
    while (!m_stopRequested.load()) {
        const auto frame = m_source.next();
        if (!frame || frame->mat.empty()) {
            if (!m_stopRequested.load())
                emit errorOccurred(tr("摄像头读取失败"));
            break;
        }
        try {
            const std::vector<FaceResult> faces = m_detector->detect(frame->mat, 0.6f, 0.3f, 5000);
            const QImage image = annotate(frame->mat, faces);
            std::lock_guard<std::mutex> lock(m_imageMutex);
            m_latest = image;
        } catch (const std::exception &error) {
            emit errorOccurred(QString::fromLocal8Bit(error.what()));
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    m_running.store(false);
}

QImage DetectorWorker::annotate(const cv::Mat &frame, const std::vector<FaceResult> &faces)
{
    cv::Mat canvas = frame.clone();
    for (const FaceResult &detection : faces) {
        const cv::Rect face(static_cast<int>(detection.box.x),
                            static_cast<int>(detection.box.y),
                            static_cast<int>(detection.box.width),
                            static_cast<int>(detection.box.height));
        cv::rectangle(canvas, face, cv::Scalar(0, 255, 0), 2);
        for (int point = 0; point < 5; ++point) {
            const cv::Point landmark(static_cast<int>(detection.landmarks[point].x),
                                     static_cast<int>(detection.landmarks[point].y));
            cv::circle(canvas, landmark, 3, cv::Scalar(0, 0, 255), cv::FILLED);
        }
        const std::array<cv::Rect2f, 2> eyeRois = computeEyeRois(detection, 0.6f);
        const cv::Rect right(static_cast<int>(eyeRois[0].x), static_cast<int>(eyeRois[0].y),
                             static_cast<int>(eyeRois[0].width), static_cast<int>(eyeRois[0].height));
        const cv::Rect left(static_cast<int>(eyeRois[1].x), static_cast<int>(eyeRois[1].y),
                            static_cast<int>(eyeRois[1].width), static_cast<int>(eyeRois[1].height));
        cv::rectangle(canvas, right, cv::Scalar(255, 0, 0), 2);
        cv::rectangle(canvas, left, cv::Scalar(0, 0, 255), 2);
        cv::putText(canvas, "R", right.tl() + cv::Point(0, -4),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 0), 2);
        cv::putText(canvas, "L", left.tl() + cv::Point(0, -4),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    }
    return labqt::toQImage(canvas);
}
