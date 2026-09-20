#include "measurement_worker.h"

#include <memory>
#include <thread>
#include <utility>

#include <QtGlobal>
#include <opencv2/imgproc.hpp>

#include "qt/logging.h"

namespace {




labcore::FramePtr mirrorHorizontal(const labcore::Frame &frame)
{
    cv::Mat mirrored;
    cv::flip(frame.mat, mirrored, 1);
    return std::make_shared<labcore::Frame>(labcore::makeFrame(std::move(mirrored), frame.seq));
}

}

MeasurementWorker::~MeasurementWorker()
{
    // 先请求并等待预览线程退出，再释放摄像头，避免线程访问已失效的设备句柄。
    stopPreview();
    close();
}

bool MeasurementWorker::open(std::string *error)
{
    if (isOpen())
        return true;
    if (!m_source.open()) {
        m_error = m_source.lastError().toStdString();
        if (error)
            *error = m_error;
        return false;
    }
    m_open.store(true);
    qCInfo(labCamera) << "opened" << m_source.description();
    return true;
}

void MeasurementWorker::close()
{
    stopPreview();
    if (m_open.exchange(false)) {
        m_source.release();
        qCInfo(labCamera) << "closed";
    }
}

bool MeasurementWorker::startPreview(PreviewCallback onFrame)
{
    if (previewRunning())
        return true;
    if (!isOpen()) {
        std::lock_guard<std::mutex> lock(m_errorMutex);
        m_error = "camera is not open";
        return false;
    }
    m_onFrame = std::move(onFrame);
    m_stopRequested.store(false);
    m_previewRunning.store(true);
    m_previewWorker.start([this] { runPreview(); });
    return true;
}

void MeasurementWorker::stopPreview()
{
    m_stopRequested.store(true);
    m_previewWorker.join();
    m_previewRunning.store(false);
}

void MeasurementWorker::setParams(const DetectParams &params)
{
    // 参数在 GUI 线程更新、采集线程读取；复制小型配置快照比长期持锁更安全。
    std::lock_guard<std::mutex> lock(m_paramsMutex);
    m_params = params;
}

DetectParams MeasurementWorker::params() const
{
    std::lock_guard<std::mutex> lock(m_paramsMutex);
    return m_params;
}

EyeDetectionResult MeasurementWorker::detect(const labcore::FramePtr &frame) const
{
    if (!frame)
        return EyeDetectionResult{};
    return detectDarkEyeTarget(frame->mat, params());
}

void MeasurementWorker::runPreview()
{

    while (!m_stopRequested.load()) {
        std::optional<labcore::Frame> frame = m_source.next();
        if (!frame)
            break;
        labcore::FramePtr shared = mirrorHorizontal(*frame);
        const EyeDetectionResult detection = detect(shared);
        if (m_onFrame)
            m_onFrame(shared, detection);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    m_previewRunning.store(false);
}

std::optional<EyeDetectionResult> MeasurementWorker::measureOnce(std::chrono::milliseconds timeout)
{
    if (!isOpen() || timeout.count() <= 0)
        return std::nullopt;

    const auto deadline = std::chrono::steady_clock::now() + timeout;
    bool discardedFirstFrame = false;
    while (!m_stopRequested.load() && std::chrono::steady_clock::now() < deadline) {
        const std::optional<labcore::Frame> frame = m_source.next();
        if (!frame)
            return std::nullopt;
        if (!discardedFirstFrame) {
            discardedFirstFrame = true;
            continue;
        }

        const EyeDetectionResult detection = detect(
            std::make_shared<const labcore::Frame>(std::move(*frame)));
        if (detection.found)
            return detection;
    }
    return std::nullopt;
}
