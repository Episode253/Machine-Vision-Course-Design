#include "low_vision_viewmodel.h"

#include <algorithm>
#include <chrono>
#include <utility>

#include <QTimer>

#include "blur_two/blur_two.h"
#include "cataract/cataract.h"
#include "glare/glare.h"
#include "qt/qt_interop.h"
#include "visual_field_loss/visual_field_loss.h"

#include <opencv2/imgproc.hpp>

LowVisionViewModel::LowVisionViewModel(QObject *parent)
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

LowVisionViewModel::~LowVisionViewModel()
{
    stop();
}

bool LowVisionViewModel::startCamera()
{
    return start(false, QString());
}

bool LowVisionViewModel::openImage(const QString &path)
{
    return path.isEmpty() ? false : start(true, path);
}

bool LowVisionViewModel::start(bool imageMode, const QString &path)
{
    stop();
    {
        std::lock_guard<std::mutex> lock(m_imageMutex);
        m_latest = QImage();
    }
    m_stopRequested.store(false);
    m_running.store(true);
    m_worker = std::thread(&LowVisionViewModel::workerLoop, this, imageMode, path);
    m_timer->start();
    emit stateChanged(imageMode ? QStringLiteral("图片处理中") : QStringLiteral("摄像头运行中"));
    return true;
}

void LowVisionViewModel::stop()
{
    m_stopRequested.store(true);
    if (m_worker.joinable())
        m_worker.join();
    m_source.release();
    m_running.store(false);
    if (m_timer)
        m_timer->stop();
    emit stateChanged(QStringLiteral("已停止"));
}

void LowVisionViewModel::workerLoop(bool imageMode, QString path)
{
    std::string error;
    bool opened = false;
    if (imageMode) {
        opened = m_source.openImage(path);
    } else {
        opened = m_source.open();
    }
    if (!opened) {
        emit errorOccurred(m_source.lastError());
        m_running.store(false);
        return;
    }

    while (!m_stopRequested.load()) {
        const auto frame = m_source.next();
        if (!frame || frame->mat.empty()) {
            if (!m_stopRequested.load())
                emit errorOccurred(m_source.lastError());
            break;
        }
        Params params;
        {
            std::lock_guard<std::mutex> lock(m_paramMutex);
            params = m_params;
        }
        const cv::Mat output = process(frame->mat, params);
        publish(labqt::toQImage(output));
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    m_source.release();
    m_running.store(false);
}

cv::Mat LowVisionViewModel::process(const cv::Mat &source, const Params &params) const
{
    cv::Mat result = source.clone();
    result = lab3::blur_two::apply(result, params.blur);
    result = lab3::visual_field_loss::apply(result, params.fieldLoss);
    result = lab3::cataract::apply(result, params.cataract);
    result = lab3::glare::apply(result, params.glare);
    if (m_sobelEnabled.load()) {
        cv::Mat gray;
        cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
        cv::Mat edges;
        cv::Sobel(gray, edges, CV_16S, 1, 0, 3);
        cv::convertScaleAbs(edges, result);
    }
    return result;
}

void LowVisionViewModel::publish(const QImage &image)
{
    if (image.isNull())
        return;
    std::lock_guard<std::mutex> lock(m_imageMutex);
    m_latest = image;
}

void LowVisionViewModel::setParam(int Params::*member, int value)
{
    std::lock_guard<std::mutex> lock(m_paramMutex);
    this->m_params.*member = std::clamp(value, 0, 100);
}

void LowVisionViewModel::setBlur(int value) { setParam(&Params::blur, value); }
void LowVisionViewModel::setFieldLoss(int value) { setParam(&Params::fieldLoss, value); }
void LowVisionViewModel::setCataract(int value) { setParam(&Params::cataract, value); }
void LowVisionViewModel::setGlare(int value) { setParam(&Params::glare, value); }
void LowVisionViewModel::setSobel(bool enabled) { m_sobelEnabled.store(enabled); }
