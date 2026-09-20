#ifndef LAB6_CALIBRATION_VIEWMODEL_H
#define LAB6_CALIBRATION_VIEWMODEL_H

#include <atomic>
#include <chrono>

#include <QImage>
#include <QMutex>
#include <QObject>
#include <QString>

#include "calibration_session.h"
#include "measurement_worker.h"
#include "qt/latest_image_slot.h"
#include "runtime/worker_thread.h"

class QTimer;



class CalibrationViewModel : public QObject {
    Q_OBJECT

public:
    explicit CalibrationViewModel(QObject *parent = nullptr);
    ~CalibrationViewModel() override;

    bool previewRunning() const;
    bool calibrated() const;

public slots:
    void startCalibration();


    void confirmCentre();
    void stopCalibration();
    void measure();
    void setRoi(int x1, int y1, int x2, int y2);
    void setThreshold(int value);

signals:
    void previewFrame(const QImage &image);


    void detectionFrame(const QImage &image);
    void previewActiveChanged(bool active);
    void calibrationFinished(const QString &centreText);


    void deviationChanged(bool found, int dx, int dy);


    void measurementFinished(const QString &centreText, const QString &actualText,
                             const QString &verdictText, bool valid);
    void errorOccurred(const QString &message);

private slots:
    void pullFrame();

private:
    static QString positionText(const cv::Point &point);
    static QString verdictText(lab6::CalibrationOutcome::Status status);

    MeasurementWorker m_worker;
    lab6::CalibrationSession m_session;
    mutable QMutex m_sessionMutex;
    labqt::LatestImageSlot m_latest;
    labqt::LatestImageSlot m_latestDetection;
    QTimer *m_frameTimer = nullptr;
    labruntime::WorkerThread m_measureWorker;
    DetectParams m_params;



    mutable QMutex m_lockMutex;
    bool m_centreLocked = false;
    cv::Point m_lockedCentre{};

    std::atomic_bool m_lastFound{false};
    std::atomic_int m_lastX{0};
    std::atomic_int m_lastY{0};
};

#endif
