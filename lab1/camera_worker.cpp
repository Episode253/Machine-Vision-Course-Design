#include "camera_worker.h"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include <opencv2/imgproc.hpp>

#include "qt/logging.h"

// 析构时确保采集线程已退出、摄像头已释放；由持有者（DisplayViewModel）在 GUI 线程销毁。
CameraWorker::~CameraWorker()
{
    stop();
}

// 在 GUI 线程调用：校验回调并复位标志后启动工作线程；返回 false 表示没能启动。
bool CameraWorker::start(FrameCallback onFrame)
{
    // 采集线程只负责取帧；界面更新通过回调交给上层定时器完成，避免跨线程直接操作 UI。
    // 已在运行就直接返回成功，重复点「开启摄像头」不会起出第二条线程。
    if (m_running.load())
        return true;
    if (!onFrame) {
        // 没有回调就无法把帧交给界面，属于调用方错误，记下错误并拒绝启动。
        std::lock_guard<std::mutex> lock(m_errorMutex);
        m_error = "no frame callback configured";
        return false;
    }
    // 回调在启动线程之前赋值，此后采集线程只读，因此不需要加锁。
    m_onFrame = std::move(onFrame);
    m_stopRequested.store(false);
    m_running.store(true);
    // 打开摄像头放在工作线程里做：open() 可能阻塞数百毫秒，不能占住 GUI 线程。
    m_worker.start([this] {
        if (!m_source.open()) {
            {
                // m_error 会被 GUI 线程的 lastError() 读到，写入必须持锁。
                std::lock_guard<std::mutex> lock(m_errorMutex);
                m_error = m_source.lastError().toStdString();
            }
            m_running.store(false);
            // 回调传 nullptr 通知界面「采集启动失败」，界面据此显示错误。
            m_onFrame(nullptr);
            return;
        }

        qCInfo(labCamera) << "preview started on" << m_source.description();
        run();
    });
    return true;
}

// 在 GUI 线程调用：先置停止标志让采集循环退出，再 join 等线程真正结束。
void CameraWorker::stop()
{
    m_stopRequested.store(true);
    m_worker.join();

    // join 返回后采集线程已不再访问 m_source，此时释放摄像头才是安全的。
    // 用 exchange 一次取出并复位运行标志，避免「取旧值」与「复位」之间被别的线程插入。
    const bool wasRunning = m_running.exchange(false);
    m_source.release();
    if (wasRunning)
        qCInfo(labCamera) << "preview stopped";
}

bool CameraWorker::sourceOpened() const
{
    return m_source.isOpened();
}

// GUI 线程读取最近一次错误；写入发生在采集线程，故与写入端共用同一把锁。
std::string CameraWorker::lastError() const
{
    std::lock_guard<std::mutex> lock(m_errorMutex);
    return m_error;
}

// 采集线程的循环：取帧、水平镜像后发布，再按 33ms 节拍补偿式等待，直到 m_stopRequested 置位。
void CameraWorker::run()
{
    // 该循环在后台线程执行，退出条件统一由原子标志控制。

    // 目标周期 33ms 约合 30fps；配合 steady_clock 扣除已耗时，所以是补偿式定时而非简单 sleep(33)。
    constexpr auto kFramePeriod = std::chrono::milliseconds(33);
    while (!m_stopRequested.load()) {
        // 记录本轮起点，稍后据此算出还需要补睡多久。
        const auto begin = std::chrono::steady_clock::now();

        std::optional<labcore::Frame> frame = m_source.next();
        if (!frame) {
            // 若是我们自己请求停止，属于正常收尾，不必记错误。
            if (!m_stopRequested.load()) {
                std::lock_guard<std::mutex> lock(m_errorMutex);
                m_error = m_source.lastError().toStdString();
                qCWarning(labCamera) << "frame read failed" << m_source.lastError();
            }
            // 连续取不到帧通常意味着设备已断开，退出循环而不是继续空转。
            break;
        }

        // 最后一个参数 1 = 绕 y 轴翻转即左右镜像，符合前置摄像头预览习惯。
        cv::flip(frame->mat, frame->mat, 1);
        // 打包成 const 共享帧并把帧内容 move 出去：帧不可变，跨层零拷贝传递，因此无需加锁。
        m_onFrame(std::make_shared<const labcore::Frame>(std::move(*frame)));

        // 还需等待的时间 = 目标周期 - 本轮(取帧+镜像+回调)已耗时。
        const auto elapsed = std::chrono::steady_clock::now() - begin;
        const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(
                                   kFramePeriod - elapsed);
        // 已经超时就不再等待，直接进入下一轮（表现为自动丢帧），避免节拍越拖越慢。
        if (remaining.count() > 0)
            std::this_thread::sleep_for(remaining);
    }

    // 无论正常停止还是取帧失败退出，都在这里统一复位运行标志。
    m_running.store(false);
}
