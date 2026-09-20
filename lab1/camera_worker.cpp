#include "camera_worker.h"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include <opencv2/imgproc.hpp>

#include "qt/logging.h"

CameraWorker::~CameraWorker()
{
    stop();
}

bool CameraWorker::start(FrameCallback onFrame)
{
    // 采集线程只负责取帧；界面更新通过回调交给上层定时器完成，避免跨线程直接操作 UI。
    if (m_running.load())
        return true;
    if (!onFrame) {
        std::lock_guard<std::mutex> lock(m_errorMutex);
        m_error = "no frame callback configured";
        return false;
    }
    m_onFrame = std::move(onFrame);
    m_stopRequested.store(false);
    m_running.store(true);
    m_worker.start([this] {
        if (!m_source.open()) {
            {
                std::lock_guard<std::mutex> lock(m_errorMutex);
                m_error = m_source.lastError().toStdString();
            }
            m_running.store(false);
            m_onFrame(nullptr);
            return;
        }

        qCInfo(labCamera) << "preview started on" << m_source.description();
        run();
    });
    return true;
}

void CameraWorker::stop()
{
    m_stopRequested.store(true);
    m_worker.join();


    const bool wasRunning = m_running.exchange(false);
    m_source.release();
    if (wasRunning)
        qCInfo(labCamera) << "preview stopped";
}

bool CameraWorker::sourceOpened() const
{
    return m_source.isOpened();
}

std::string CameraWorker::lastError() const
{
    std::lock_guard<std::mutex> lock(m_errorMutex);
    return m_error;
}

void CameraWorker::run()
{
    // 该循环在后台线程执行，退出条件统一由原子标志控制。


    constexpr auto kFramePeriod = std::chrono::milliseconds(33);
    while (!m_stopRequested.load()) {
        const auto begin = std::chrono::steady_clock::now();

        std::optional<labcore::Frame> frame = m_source.next();
        if (!frame) {
            if (!m_stopRequested.load()) {
                std::lock_guard<std::mutex> lock(m_errorMutex);
                m_error = m_source.lastError().toStdString();
                qCWarning(labCamera) << "frame read failed" << m_source.lastError();
            }
            break;
        }

        cv::flip(frame->mat, frame->mat, 1);
        m_onFrame(std::make_shared<const labcore::Frame>(std::move(*frame)));

        const auto elapsed = std::chrono::steady_clock::now() - begin;
        const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(
                                   kFramePeriod - elapsed);
        if (remaining.count() > 0)
            std::this_thread::sleep_for(remaining);
    }

    m_running.store(false);
}
