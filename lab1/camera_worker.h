#ifndef LAB1_CAMERA_WORKER_H
#define LAB1_CAMERA_WORKER_H

#include <atomic>
#include <functional>
#include <mutex>
#include <string>

#include "core/frame.hpp"
#include "runtime/frame_source.h"
#include "runtime/worker_thread.h"




class CameraWorker {
public:
    using FrameCallback = std::function<void(labcore::FramePtr)>;

    CameraWorker() = default;
    ~CameraWorker();

    CameraWorker(const CameraWorker &) = delete;
    CameraWorker &operator=(const CameraWorker &) = delete;



    bool start(FrameCallback onFrame);
    void stop();
    bool running() const { return m_running.load(); }

    bool sourceOpened() const;
    std::string lastError() const;

private:
    void run();

    labruntime::WorkerThread m_worker;
    FrameSource m_source;
    FrameCallback m_onFrame;
    std::atomic_bool m_stopRequested{false};
    std::atomic_bool m_running{false};
    mutable std::mutex m_errorMutex;
    std::string m_error;
};

#endif
