#ifndef LAB7_DETECTOR_WORKER_H
#define LAB7_DETECTOR_WORKER_H

#include <atomic>
#include <mutex>
#include <thread>

#include <QImage>
#include <QObject>
#include <QString>
#include <opencv2/core.hpp>

#include "runtime/frame_source.h"
#include "yunet_trt.hpp"

class QTimer;

// DetectorWorker 是 QObject，但亲和创建它的线程（在主窗口里就是主线程）：start()/stop() 槽、
// 内部的 QTimer 都随主线程执行，真正跑在工作线程的只有 loop()。
// 工作线程在正常帧通路上从不 emit，只往 m_latest 写；由主线程定时器取出后再 emit frameChanged，
// 这样 QPixmap/QLabel 始终只在 GUI 线程被碰。
class DetectorWorker : public QObject {
    Q_OBJECT
public:
    explicit DetectorWorker(QObject *parent = nullptr);
    ~DetectorWorker() override;

    bool running() const { return m_running.load(); }

public slots:
    // 两个槽都在主线程被调用：start() 里初始化并起工作线程，stop() 里 join 工作线程。
    void start();
    void stop();

signals:
    // 都在主线程发出，只有 loop() 错误路径上的 errorOccurred 是从工作线程经队列连接投递过来的。
    void frameChanged(const QImage &image);
    void statusChanged(const QString &status);
    void errorOccurred(const QString &message);

private:
    // 工作线程的主循环，只被 start() 起的那个 std::thread 执行。
    void loop();
    // 纯函数式的绘制辅助：不访问成员，只读参数，因此可以声明为 static 在工作线程里直接调用。
    static QImage annotate(const cv::Mat &frame, const std::vector<FaceResult> &faces);

    // 摄像头：start() 在主线程 open()，此后只有工作线程调用 next()，stop() 才 release()。
    FrameSource m_source;
    // TensorRT 检测器，独占 GPU 资源；stop() 时 reset() 释放，init 失败时也立即 reset()。
    std::unique_ptr<YuNetTRT> m_detector;
    // 定时器随本对象留在主线程，负责把工作线程产出的最新帧转发给界面。
    QTimer *m_timer = nullptr;
    // 工作线程句柄；stop() 与析构里 join，join 前先置 m_stopRequested 让循环退出。
    std::thread m_thread;
    // 两个标志都跨线程读写，因此用 atomic：m_running 供界面查询，m_stopRequested 供工作线程观察退出。
    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
    // 保护 m_latest：工作线程写、主线程定时器取走，只有这一段需要加锁。
    mutable std::mutex m_imageMutex;
    QImage m_latest;
};

#endif
