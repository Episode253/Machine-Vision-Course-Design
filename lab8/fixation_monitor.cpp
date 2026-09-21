#include "fixation_monitor.h"

#include <algorithm>
#include <chrono>
#include <QFileInfo>
#include <thread>

#include <QCoreApplication>
#include <QStringList>
#include <opencv2/imgproc.hpp>

#include "qt/qt_interop.h"

namespace {

QString findModel(const QString &relative)
{
    const QStringList candidates{
        relative,
        QCoreApplication::applicationDirPath() + "/../" + relative,
        QCoreApplication::applicationDirPath() + "/../../" + relative};
    for (const QString &candidate : candidates)
        if (QFileInfo(candidate).isFile())
            return candidate;
    return {};
}

cv::Rect clippedRect(const cv::Rect2f &rect, const cv::Size &size)
{
    return cv::Rect(static_cast<int>(rect.x), static_cast<int>(rect.y),
                    static_cast<int>(rect.width), static_cast<int>(rect.height))
        & cv::Rect(0, 0, size.width, size.height);
}

bool findPupil(const cv::Mat &frame, const cv::Rect &roi, cv::Point *center)
{
    if (!center || roi.empty())
        return false;
    cv::Mat gray;
    if (frame.channels() == 1)
        gray = frame(roi);
    else if (frame.channels() == 3)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGR2GRAY);
    else if (frame.channels() == 4)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGRA2GRAY);
    else
        return false;

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0.0);
    cv::Mat binary;
    cv::threshold(gray, binary, 55, 255, cv::THRESH_BINARY_INV);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        return false;
    const auto largest = std::max_element(contours.begin(), contours.end(),
        [](const auto &left, const auto &right) {
            return cv::contourArea(left) < cv::contourArea(right);
        });
    const cv::Rect bounds = cv::boundingRect(*largest);
    if (bounds.area() <= 0)
        return false;
    *center = roi.tl() + bounds.tl() + cv::Point(bounds.width / 2, bounds.height / 2);
    return true;
}

}

FixationMonitor::FixationMonitor(QObject *parent) : QObject(parent) {}

FixationMonitor::~FixationMonitor() { stop(); }

void FixationMonitor::start()
{
    if (m_running.load())
        return;

    const QString onnx = findModel("yunet_trt_cpp/models/face_detection_yunet_2023mar.onnx");
    const QString engine = findModel("yunet_trt_cpp/models/yunet_fp16.engine");
    if (onnx.isEmpty() || engine.isEmpty()) {
        emit errorOccurred(tr("找不到 YuNet ONNX 或 TensorRT engine"));
        return;
    }

    try {
        m_detector = std::make_unique<YuNetTRT>();
        if (!m_detector->init(onnx.toStdString(), engine.toStdString(), true, false)) {
            emit errorOccurred(tr("YuNet TensorRT 初始化失败"));
            m_detector.reset();
            return;
        }
    } catch (const std::exception &error) {
        emit errorOccurred(QString::fromLocal8Bit(error.what()));
        m_detector.reset();
        return;
    }

    if (!m_source.open()) {
        emit errorOccurred(m_source.lastError());
        m_detector.reset();
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        m_haveCentre = false;
        m_lastValid = false;
    }
    m_stopRequested.store(false);
    m_running.store(true);
    m_thread = std::thread(&FixationMonitor::loop, this);
}

void FixationMonitor::stop()
{
    m_stopRequested.store(true);
    if (m_thread.joinable())
        m_thread.join();
    m_source.release();
    m_detector.reset();
    m_running.store(false);

    bool emitInvalid = false;
    {
        std::lock_guard<std::mutex> lock(m_stateMutex);
        emitInvalid = m_lastValid;
        m_lastValid = false;
        m_haveCentre = false;
    }
    if (emitInvalid)
        emit fixationChanged(false);
}

void FixationMonitor::loop()
{
    while (!m_stopRequested.load()) {
        const auto frame = m_source.next();
        if (!frame || frame->mat.empty()) {
            if (!m_stopRequested.load())
                emit errorOccurred(tr("摄像头读取失败"));
            break;
        }

        bool valid = false;
        cv::Mat canvas = frame->mat.clone();
        try {
            const std::vector<FaceResult> faces = m_detector->detect(frame->mat, 0.6f, 0.3f, 20);
            if (!faces.empty()) {
                const FaceResult &face = *std::max_element(
                    faces.begin(), faces.end(), [](const FaceResult &left, const FaceResult &right) {
                        return left.score < right.score;
                    });
                const auto eyes = computeEyeRois(face, 0.6f);
                std::array<cv::Point, 2> pupils{};
                bool bothFound = true;
                for (int index = 0; index < 2; ++index) {
                    const cv::Rect roi = clippedRect(eyes[static_cast<std::size_t>(index)], frame->mat.size());
                    if (roi.empty()) { bothFound = false; continue; }
                    cv::rectangle(canvas, roi, index == 0 ? cv::Scalar(255, 0, 0) : cv::Scalar(0, 0, 255), 2);
                    if (!findPupil(frame->mat, roi, &pupils[static_cast<std::size_t>(index)])) {
                        bothFound = false;
                        continue;
                    }
                    cv::drawMarker(canvas, pupils[static_cast<std::size_t>(index)], cv::Scalar(0, 255, 255), cv::MARKER_CROSS, 10, 2);
                }
                if (bothFound) {
                    const cv::Point current((pupils[0].x + pupils[1].x) / 2, (pupils[0].y + pupils[1].y) / 2);
                    std::lock_guard<std::mutex> lock(m_stateMutex);
                    if (!m_haveCentre) { m_centre = current; m_haveCentre = true; }
                    valid = cv::norm(current - m_centre) < 22.0;
                }
                const cv::Rect faceRect = clippedRect(face.box, frame->mat.size());
                cv::rectangle(canvas, faceRect, cv::Scalar(0, 255, 0), 2);
            }
        } catch (const std::exception &error) {
            emit errorOccurred(QString::fromLocal8Bit(error.what()));
            break;
        }

        bool changed = false;
        {
            std::lock_guard<std::mutex> lock(m_stateMutex);
            changed = valid != m_lastValid;
            m_lastValid = valid;
        }
        if (changed)
            emit fixationChanged(valid);
        emit frameChanged(labqt::toQImage(canvas));
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    m_running.store(false);
}
