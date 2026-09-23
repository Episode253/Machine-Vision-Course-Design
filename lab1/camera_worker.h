#ifndef LAB1_CAMERA_WORKER_H
#define LAB1_CAMERA_WORKER_H

#include <atomic>
#include <functional>
#include <mutex>
#include <string>

#include "core/frame.hpp"
#include "runtime/frame_source.h"
#include "runtime/worker_thread.h"




// lab1 的摄像头采集器：独占一条工作线程做取帧，通过回调把共享帧交给上层。
// 线程归属：start()/stop()/错误查询在 GUI 线程调用，run() 在工作线程执行。
class CameraWorker {
public:
    // 回调参数为 nullptr 表示「采集启动失败」，而不是「这一帧为空」。
    using FrameCallback = std::function<void(labcore::FramePtr)>;

    CameraWorker() = default;
    ~CameraWorker();

    CameraWorker(const CameraWorker &) = delete;
    CameraWorker &operator=(const CameraWorker &) = delete;



    // 启动采集线程；重复调用是幂等的。
    bool start(FrameCallback onFrame);
    // 停止并等待采集线程退出，同时释放摄像头设备。
    void stop();
    bool running() const { return m_running.load(); }

    bool sourceOpened() const;
    std::string lastError() const;

private:
    // 采集线程入口，只允许工作线程执行。
    void run();

    labruntime::WorkerThread m_worker;
    FrameSource m_source;
    // 上层提供的帧回调：启动线程之前赋值，之后采集线程只读，因此不必加锁。
    FrameCallback m_onFrame;
    // 停止请求：GUI 线程写、采集线程每轮读，用原子量即可，不必让循环去抢锁。
    std::atomic_bool m_stopRequested{false};
    // 是否正在采集：start()/stop() 与 run() 之间共享。
    std::atomic_bool m_running{false};
    // m_error 跨线程访问（采集线程写、GUI 线程读），用互斥量保护。
    mutable std::mutex m_errorMutex;
    std::string m_error;
};

#endif
