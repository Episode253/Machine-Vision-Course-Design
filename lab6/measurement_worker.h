#ifndef LAB6_MEASUREMENT_WORKER_H
#define LAB6_MEASUREMENT_WORKER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "core/frame.hpp"
#include "eye_detection.h"
#include "runtime/frame_source.h"
#include "runtime/worker_thread.h"

// 摄像头采集与瞳孔检测的封装：open/close/startPreview/stopPreview 由界面线程调用，
// runPreview() 跑在 m_previewWorker 的线程里，measureOnce() 由调用方决定跑在哪个线程。
// 预览与测量共用同一个 FrameSource，不能同时取帧（调用方需先停预览再测量）。
class MeasurementWorker {
public:
    using PreviewCallback = std::function<void(labcore::FramePtr, const EyeDetectionResult &)>;

    MeasurementWorker() = default;
    ~MeasurementWorker();

    MeasurementWorker(const MeasurementWorker &) = delete;
    MeasurementWorker &operator=(const MeasurementWorker &) = delete;

    bool open(std::string *error);
    void close();
    bool isOpen() const { return m_open.load(); }



    // 预览：startPreview 注册每帧回调（回调在预览线程内执行），
    // stopPreview 请求停止并 join，返回时预览线程已退出。
    bool startPreview(PreviewCallback onFrame);
    void stopPreview();
    bool previewRunning() const { return m_previewRunning.load(); }


    // 参数：界面线程写，采集与测量线程读，内部用 m_paramsMutex 保护，不长期持锁。
    void setParams(const DetectParams &params);
    DetectParams params() const;






    // 在超时内反复取帧做一次检测：拿到合格结果立即返回，超时或收到停止请求则返回 nullopt。
    // 阻塞调用，返回时摄像头不保证已停止。
    std::optional<EyeDetectionResult> measureOnce(std::chrono::milliseconds timeout);

private:
    void runPreview();
    EyeDetectionResult detect(const labcore::FramePtr &frame) const;

    FrameSource m_source;
    // 持有并 join 预览线程，析构时自动收尾。
    labruntime::WorkerThread m_previewWorker;
    // 每帧回调，在预览线程内被调用；startPreview 时写入，之后只读，故不必加锁。
    PreviewCallback m_onFrame;
    // 停止请求标志：预览循环与 measureOnce 的循环共用；stopPreview 置位、startPreview 清位。
    std::atomic_bool m_stopRequested{false};
    // 预览线程是否在运行（startPreview 置位，循环退出或 stopPreview 清位），供界面查询。
    std::atomic_bool m_previewRunning{false};
    // 摄像头句柄是否已打开；isOpen() 会被别的线程调用，所以用原子量。
    std::atomic_bool m_open{false};

    // 保护 m_params：写入发生在界面线程，读取发生在采集与测量线程。
    mutable std::mutex m_paramsMutex;
    DetectParams m_params;

    // 最近一次错误文本；startPreview 的失败路径在锁内写入。
    mutable std::mutex m_errorMutex;
    std::string m_error;
};

#endif
