#include "measurement_worker.h"

#include <memory>
#include <thread>
#include <utility>

#include <QtGlobal>
#include <opencv2/imgproc.hpp>

#include "qt/logging.h"

namespace {

// 仅供本文件使用的辅助函数：放匿名命名空间避免符号外泄。
// 水平镜像：与 lab1 的预览一致，符合前置摄像头的镜面显示习惯；
// 镜像后仍是同一帧，所以沿用原帧序号。
labcore::FramePtr mirrorHorizontal(const labcore::Frame &frame)
{
    cv::Mat mirrored;
    // flipCode = 1 表示绕 y 轴翻转，即左右镜像。
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
    // 已打开就算成功，重复调用是幂等的（界面可能多次点“开始校准”）。
    if (isOpen())
        return true;
    if (!m_source.open()) {
        // 底层错误既留一份在成员里，也按需回传给调用方用于界面提示。
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
    // 先停预览线程再放句柄：预览循环正在使用 m_source，先 join 才不会访问已释放的设备。
    stopPreview();
    // exchange 保证并发下只有一次真正执行释放。
    if (m_open.exchange(false)) {
        m_source.release();
        qCInfo(labCamera) << "closed";
    }
}

bool MeasurementWorker::startPreview(PreviewCallback onFrame)
{
    // 已在预览就直接成功返回，避免重复起线程。
    if (previewRunning())
        return true;
    if (!isOpen()) {
        std::lock_guard<std::mutex> lock(m_errorMutex);
        m_error = "camera is not open";
        return false;
    }
    m_onFrame = std::move(onFrame);
    // 顺序要紧：先装回调，再清停止标志，最后置 running 并起线程，
    // 否则新线程可能在新状态就绪之前就开始跑。
    m_stopRequested.store(false);
    m_previewRunning.store(true);
    m_previewWorker.start([this] { runPreview(); });
    return true;
}

void MeasurementWorker::stopPreview()
{
    // 置位后 join，等预览线程跑完当前一轮自行退出；本调用会阻塞到线程结束
    //（最多一帧处理时间 + 20ms 节拍）。
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

// 单帧检测的入口：预览与测量两条路径都走这里，保证参数与算法完全一致。
// 传入空指针时返回“未找到”的空结果，而不是抛错。
EyeDetectionResult MeasurementWorker::detect(const labcore::FramePtr &frame) const
{
    if (!frame)
        return EyeDetectionResult{};
    // params() 按值返回一份快照，检测期间即使界面改了阈值也不影响这一帧。
    return detectDarkEyeTarget(frame->mat, params());
}

// 预览线程主体：取帧 → 水平镜像 → 检测 → 回调，节拍 20ms（约 50Hz），
// 比界面 33ms 的刷新快，多出来的帧由 LatestImageSlot 的“只留最新”语义自然丢弃。
void MeasurementWorker::runPreview()
{
    while (!m_stopRequested.load()) {
        std::optional<labcore::Frame> frame = m_source.next();
        if (!frame)
            // 取帧失败（设备出错或句柄已失效）就退出循环，不做无意义的空转。
            break;
        labcore::FramePtr shared = mirrorHorizontal(*frame);
        const EyeDetectionResult detection = detect(shared);
        // 回调在本线程内执行，实现方需自行保证共享状态的同步。
        if (m_onFrame)
            m_onFrame(shared, detection);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    // 循环以任何方式退出后都要清位，让 previewRunning() 如实反映线程已经结束。
    m_previewRunning.store(false);
}

// 单次测量：在超时窗口内反复取帧检测，拿到第一个成功结果就返回（不等满超时）。
// 阻塞调用，跑在调用方准备的线程里；返回 nullopt 表示超时、被请求停止或摄像头不可用。
std::optional<EyeDetectionResult> MeasurementWorker::measureOnce(std::chrono::milliseconds timeout)
{
    // 没打开或超时非正数时没有可用的帧，不必进入循环。
    if (!isOpen() || timeout.count() <= 0)
        return std::nullopt;

    // 用 steady_clock 算绝对截止时刻：循环里每次取帧的耗时不同，
    // 按绝对时刻比较才能保证总时长接近 timeout。
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    // 首帧丢弃用标志位实现，而不是“先单独取一帧再进循环”——
    // 首帧什么时候到由循环里的 next() 决定，标志位写法能顺带处理取帧失败的退出。
    bool discardedFirstFrame = false;
    // 退出条件有两个：收到停止请求，或到达截止时刻。
    while (!m_stopRequested.load() && std::chrono::steady_clock::now() < deadline) {
        const std::optional<labcore::Frame> frame = m_source.next();
        if (!frame)
            // 取帧失败说明摄像头不可用，继续等到超时也没有意义。
            return std::nullopt;
        if (!discardedFirstFrame) {
            // 丢弃首帧：摄像头刚打开时自动曝光/增益尚未收敛，首帧整体偏暗，
            // 二值化结果不可靠，据此算出的瞳孔位置不能用。跳过它给后一帧留出收敛时间。
            discardedFirstFrame = true;
            continue;
        }

        // 从这一帧起才做检测；std::move 把帧的数据搬进共享帧，避免整幅图像再拷贝一次。
        // 注意本路径不经过 mirrorHorizontal，检测坐标是采集原始帧的坐标系。
        const EyeDetectionResult detection = detect(
            std::make_shared<const labcore::Frame>(std::move(*frame)));
        if (detection.found)
            return detection;
    }
    // 超时或中途被打断：两种情况下都没有可用的测量结果。
    return std::nullopt;
}
