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

class DetectorWorker : public QObject {
    Q_OBJECT
public:
    explicit DetectorWorker(QObject *parent = nullptr);
    ~DetectorWorker() override;

    bool running() const { return m_running.load(); }

public slots:
    void start();
    void stop();

signals:
    void frameChanged(const QImage &image);
    void statusChanged(const QString &status);
    void errorOccurred(const QString &message);

private:
    void loop();
    static QImage annotate(const cv::Mat &frame, const std::vector<FaceResult> &faces);

    FrameSource m_source;
    std::unique_ptr<YuNetTRT> m_detector;
    QTimer *m_timer = nullptr;
    std::thread m_thread;
    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
    mutable std::mutex m_imageMutex;
    QImage m_latest;
};

#endif
