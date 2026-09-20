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

class FixationMonitor : public QObject {
    Q_OBJECT
public:
    explicit FixationMonitor(QObject *parent = nullptr);
    ~FixationMonitor() override;
    bool active() const { return m_running.load(); }
public slots:
    void start();
    void stop();
signals:
    void fixationChanged(bool valid);
    void frameChanged(const QImage &image);
    void errorOccurred(const QString &message);
private:
    void loop();
    FrameSource m_source;
    std::unique_ptr<YuNetTRT> m_detector;
    std::thread m_thread;
    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
    std::mutex m_stateMutex;
    bool m_haveCentre = false;
    cv::Point m_centre{};
    bool m_lastValid = false;
};

#endif
