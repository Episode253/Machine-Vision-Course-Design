#ifndef LAB8_FIXATION_MONITOR_H
#define LAB8_FIXATION_MONITOR_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include <QImage>
#include <QObject>
#include <opencv2/core.hpp>

#include "runtime/frame_source.h"
#include "yunet_trt.hpp"

// 注视监视器：YuNet 检测 + 瞳孔定位 + 自标定基线，判定视线是否停留在注视点上。
// 自身不画界面，由工作线程通过 fixationChanged / frameChanged 把状态与画面交给 GUI 线程。
class FixationMonitor : public QObject {
    Q_OBJECT
public:
    explicit FixationMonitor(QObject *parent = nullptr);
    ~FixationMonitor() override;
    bool active() const { return m_running.load(); }
public slots:
    // 两个槽都由 GUI 线程直接调用（MainWindow 的按钮 clicked）；stop() 内部会 join 工作线程。
    void start();
    void stop();
signals:
    // 判定结果翻转时才发射（边沿触发）；stop() 时若原本为 true 会补发一次 false。
    void fixationChanged(bool valid);
    // 由工作线程直接 emit，携带已深拷贝的 QImage，经队列连接投递到 GUI 线程。
    void frameChanged(const QImage &image);
    void errorOccurred(const QString &message);
private:
    // 工作线程入口。
    void loop();
    // m_source/m_detector/m_thread 由 start()/stop() 串起生命周期：start() 里准备完毕才起线程，
    // stop() 里先 join 再销毁，所以这三者本身不需要额外加锁。
    FrameSource m_source;
    std::unique_ptr<YuNetTRT> m_detector;
    std::thread m_thread;
    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
    // 以下状态量工作线程会写、GUI 线程的 start()/stop() 也会读写，统一由 m_stateMutex 保护。
    std::mutex m_stateMutex;
    // 是否已完成自标定（第一帧检测到双眼瞳孔的那一刻即为基线）。
    bool m_haveCentre = false;
    // 自标定基线：首次检测到双眼时的瞳孔中点。
    cv::Point m_centre{};
    // 上一次对外发布的判定结果，用于把 fixationChanged 做成边沿触发。
    bool m_lastValid = false;
};

#endif
