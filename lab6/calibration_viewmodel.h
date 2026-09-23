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



// lab6 的逻辑层：开摄像头、跑校准预览、锁定中心、发起单次测量。
// 对象活在 GUI 线程；跨线程的部分只有预览回调（预览线程）与测量任务（测量线程），
// 两者通过 QMutex 与原子量交换状态，任何工作线程都不直接碰控件。
class CalibrationViewModel : public QObject {
    Q_OBJECT

public:
    explicit CalibrationViewModel(QObject *parent = nullptr);
    ~CalibrationViewModel() override;

    bool previewRunning() const;
    bool calibrated() const;

public slots:
    // 打开摄像头并启动预览，同时清空上一轮标定状态（GUI 线程）。
    void startCalibration();


    // 把当前瞳孔位置锁定为基准中心，之后进入“实时偏差显示”状态。
    void confirmCentre();
    void stopCalibration();
    void measure();
    void setRoi(int x1, int y1, int x2, int y2);
    void setThreshold(int value);

signals:
    // 彩色预览画面（带 ROI 框与瞳孔标注），由 pullFrame 定时器从最新帧槽取出后发出。
    void previewFrame(const QImage &image);


    // 反转二值图画面（带标注），用于观察阈值效果；测量结果也用它回显。
    void detectionFrame(const QImage &image);
    void previewActiveChanged(bool active);
    void calibrationFinished(const QString &centreText);


    // 锁定中心后每帧发出的实时偏差：dx/dy 是相对锁定中心的像素差，found 为假时两者无意义。
    void deviationChanged(bool found, int dx, int dy);


    // 单次测量结束：中心位置文案、实测位置文案、结论文案、本次测量是否有效。
    void measurementFinished(const QString &centreText, const QString &actualText,
                             const QString &verdictText, bool valid);
    void errorOccurred(const QString &message);

private slots:
    void pullFrame();

private:
    static QString positionText(const cv::Point &point);
    static QString verdictText(lab6::CalibrationOutcome::Status status);

    // 采集与检测的执行者：预览线程和摄像头句柄都在它内部。
    MeasurementWorker m_worker;
    // 校准期的累积均值：预览线程写、GUI 线程读，由下面的互斥量保护。
    lab6::CalibrationSession m_session;
    mutable QMutex m_sessionMutex;
    // 单槽“最新帧”缓冲：预览线程写、GUI 定时器取走即清空，慢的一方自然丢帧，不会积压。
    labqt::LatestImageSlot m_latest;
    labqt::LatestImageSlot m_latestDetection;
    // 33ms 定时器，仅预览期间运行，负责把槽里最新的两幅图 emit 给界面。
    QTimer *m_frameTimer = nullptr;
    // 单次测量的任务线程：measure() 时启动，析构时 join。
    labruntime::WorkerThread m_measureWorker;
    DetectParams m_params;



    // 保护 m_centreLocked / m_lockedCentre：预览线程与 GUI 线程都会访问。
    mutable QMutex m_lockMutex;
    bool m_centreLocked = false;
    cv::Point m_lockedCentre{};

    // 最近一帧的检测结果：预览线程写，GUI 线程在 confirmCentre/pullFrame 里读，
    // 用原子量省掉再加一把锁。
    std::atomic_bool m_lastFound{false};
    std::atomic_int m_lastX{0};
    std::atomic_int m_lastY{0};
};

#endif
