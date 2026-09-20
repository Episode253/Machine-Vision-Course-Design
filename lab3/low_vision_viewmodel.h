#ifndef LAB3_LOW_VISION_VIEWMODEL_H
#define LAB3_LOW_VISION_VIEWMODEL_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

#include <QImage>
#include <QObject>
#include <QString>

#include "runtime/frame_source.h"

class QTimer;

class LowVisionViewModel : public QObject {
    Q_OBJECT
public:
    explicit LowVisionViewModel(QObject *parent = nullptr);
    ~LowVisionViewModel() override;

    bool running() const { return m_running.load(); }

public slots:
    bool startCamera();
    bool openImage(const QString &path);
    void stop();
    void setBlur(int value);
    void setFieldLoss(int value);
    void setCataract(int value);
    void setGlare(int value);
    void setSobel(bool enabled);

signals:
    void frameChanged(const QImage &image);
    void stateChanged(const QString &state);
    void errorOccurred(const QString &message);

private:
    struct Params { int blur = 0; int fieldLoss = 0; int cataract = 0; int glare = 0; };
    bool start(bool imageMode, const QString &path);
    void workerLoop(bool imageMode, QString path);
    cv::Mat process(const cv::Mat &source, const Params &params) const;
    void publish(const QImage &image);
    void setParam(int Params::*member, int value);

    FrameSource m_source;
    QTimer *m_timer = nullptr;
    std::thread m_worker;
    std::atomic_bool m_running{false};
    std::atomic_bool m_stopRequested{false};
    mutable std::mutex m_paramMutex;
    Params m_params;
    std::atomic_bool m_sobelEnabled{false};
    mutable std::mutex m_imageMutex;
    QImage m_latest;
};

#endif
