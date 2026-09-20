#include "calibration_viewmodel.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <utility>

#include <QTimer>

#include <opencv2/imgproc.hpp>

#include "qt/logging.h"
#include "qt/qt_interop.h"

namespace {



const cv::Scalar kRoiColour(60, 200, 60);
const cv::Scalar kPupilColour(0, 0, 255);
const cv::Scalar kCentreColour(255, 160, 0);
const cv::Scalar kDeviationColour(0, 220, 220);

void drawFixationOverlay(cv::Mat &canvas, const cv::Point &pupil, bool found,
                         const cv::Point &centre, bool locked)
{
    if (locked) {
        cv::drawMarker(canvas, centre, kCentreColour, cv::MARKER_TILTED_CROSS, 14, 2);
        if (found)
            cv::line(canvas, centre, pupil, kDeviationColour, 2);
    }
    if (found)
        cv::drawMarker(canvas, pupil, kPupilColour, cv::MARKER_CROSS, 12, 2);
}


QImage annotatePreview(const cv::Mat &frame, const cv::Rect &roi,
                       const EyeDetectionResult &detection, bool locked,
                       const cv::Point &lockedCentre)
{
    cv::Mat canvas = frame.clone();
    if (canvas.channels() != 3)
        return labqt::toQImage(canvas);

    const cv::Rect clipped = roi & cv::Rect(0, 0, canvas.cols, canvas.rows);
    if (clipped.empty())
        return labqt::toQImage(canvas);

    cv::rectangle(canvas, clipped, kRoiColour, 2);

    drawFixationOverlay(canvas, clipped.tl() + detection.center, detection.found,
                        clipped.tl() + lockedCentre, locked);
    if (locked && detection.found) {
        char text[40];
        std::snprintf(text, sizeof(text), "dx=%+d dy=%+d",
                      detection.center.x - lockedCentre.x, detection.center.y - lockedCentre.y);
        cv::putText(canvas, text, clipped.tl() + cv::Point(4, 16), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    kDeviationColour, 1, cv::LINE_AA);
    }
    return labqt::toQImage(canvas);
}



QImage annotateDetection(const EyeDetectionResult &detection, bool locked,
                         const cv::Point &lockedCentre)
{
    if (detection.binary.empty())
        return QImage();
    cv::Mat canvas;
    cv::cvtColor(detection.binary, canvas, cv::COLOR_GRAY2BGR);
    drawFixationOverlay(canvas, detection.center, detection.found, lockedCentre, locked);
    if (locked) {
        const bool centred = detection.found
            && std::abs(detection.center.x - lockedCentre.x) < 4
            && std::abs(detection.center.y - lockedCentre.y) < 4;
        cv::rectangle(canvas, cv::Rect(0, 0, canvas.cols, canvas.rows),
                      centred ? kRoiColour : kPupilColour, 2);
    }
    return labqt::toQImage(canvas);
}

}

CalibrationViewModel::CalibrationViewModel(QObject *parent)
    : QObject(parent)
{
    m_frameTimer = new QTimer(this);
    m_frameTimer->setInterval(33);
    connect(m_frameTimer, &QTimer::timeout, this, &CalibrationViewModel::pullFrame);
}

CalibrationViewModel::~CalibrationViewModel()
{




    m_worker.stopPreview();
    m_frameTimer->stop();



    m_measureWorker.join();
    m_worker.close();
}

bool CalibrationViewModel::previewRunning() const
{
    return m_worker.previewRunning();
}

bool CalibrationViewModel::calibrated() const
{
    QMutexLocker locker(&m_sessionMutex);
    return m_session.calibrated();
}

void CalibrationViewModel::startCalibration()
{
    // 校准阶段持续取帧并累积有效瞳孔位置；确认后才锁定中心点供后续偏差判断使用。
    if (m_worker.previewRunning())
        return;



    m_measureWorker.join();

    std::string error;
    if (!m_worker.open(&error)) {
        emit errorOccurred(QString::fromStdString(error));
        return;
    }

    {
        QMutexLocker locker(&m_sessionMutex);
        m_session.reset();
    }
    {
        QMutexLocker locker(&m_lockMutex);
        m_centreLocked = false;
        m_lockedCentre = cv::Point();
    }
    m_lastFound.store(false);
    m_lastX.store(0);
    m_lastY.store(0);

    m_worker.setParams(m_params);
    const bool started = m_worker.startPreview(
        [this](labcore::FramePtr frame, const EyeDetectionResult &detection) {
            bool locked = false;
            cv::Point lockedCentre;
            {
                QMutexLocker locker(&m_lockMutex);
                locked = m_centreLocked;
                lockedCentre = m_lockedCentre;
            }


            m_lastFound.store(detection.found);
            if (detection.found) {
                m_lastX.store(detection.center.x);
                m_lastY.store(detection.center.y);
            }
            if (!locked) {
                QMutexLocker locker(&m_sessionMutex);
                m_session.observe(detection);
            }
            const cv::Rect roi = m_worker.params().roi;
            m_latest.set(annotatePreview(frame->mat, roi, detection, locked, lockedCentre));
            m_latestDetection.set(annotateDetection(detection, locked, lockedCentre));
        });
    if (!started) {
        emit errorOccurred(tr("摄像头预览启动失败"));
        return;
    }
    m_frameTimer->start();
    emit previewActiveChanged(true);
    qCInfo(labCamera) << "calibration preview started";
}

void CalibrationViewModel::confirmCentre()
{
    if (!m_worker.previewRunning()) {
        emit errorOccurred(tr("请先开始校准"));
        return;
    }


    cv::Point centre;
    bool haveCentre = false;
    if (m_lastFound.load()) {
        centre = cv::Point(m_lastX.load(), m_lastY.load());
        haveCentre = true;
    } else {
        QMutexLocker locker(&m_sessionMutex);
        haveCentre = m_session.calibrated();
        centre = m_session.centre();
    }
    if (!haveCentre) {
        emit errorOccurred(tr("尚未检测到瞳孔，无法确认中心"));
        return;
    }
    {
        QMutexLocker locker(&m_lockMutex);
        m_centreLocked = true;
        m_lockedCentre = centre;
    }

    emit calibrationFinished(positionText(centre));
    qCInfo(labCamera) << "centre locked at" << centre.x << centre.y;
}

void CalibrationViewModel::stopCalibration()
{
    if (!m_worker.previewRunning()) {
        m_latest.clear();
        m_latestDetection.clear();
        return;
    }
    m_worker.stopPreview();
    m_frameTimer->stop();


    m_latest.clear();
    m_latestDetection.clear();
    emit previewActiveChanged(false);
    qCInfo(labCamera) << "calibration preview stopped";
}

void CalibrationViewModel::measure()
{
    if (!m_worker.isOpen()) {
        emit errorOccurred(tr("摄像头未打开，请先进行中心校准"));
        return;
    }
    bool locked = false;
    {
        QMutexLocker locker(&m_lockMutex);
        locked = m_centreLocked;
    }
    if (!locked) {
        emit errorOccurred(tr("请先确认中心"));
        return;
    }
    stopCalibration();


    m_measureWorker.start([this] {
        lab6::CalibrationOutcome outcome;
        const std::optional<EyeDetectionResult> detection =
            m_worker.measureOnce(std::chrono::milliseconds(2000));
        cv::Point lockedCentre;
        {
            QMutexLocker locker(&m_lockMutex);
            lockedCentre = m_lockedCentre;
        }
        if (!detection) {
            outcome.status = lab6::CalibrationOutcome::Status::Timeout;
        } else {
            {
                QMutexLocker locker(&m_sessionMutex);
                outcome = m_session.evaluateAgainst(*detection, lockedCentre);
            }

            emit detectionFrame(annotateDetection(*detection, true, lockedCentre));
        }
        emit measurementFinished(positionText(outcome.centre), positionText(outcome.actual),
                                 verdictText(outcome.status), outcome.valid());
    });
}

void CalibrationViewModel::setRoi(int x1, int y1, int x2, int y2)
{
    m_params.roi = cv::Rect(std::min(x1, x2), std::min(y1, y2), std::abs(x1 - x2), std::abs(y1 - y2));
    m_worker.setParams(m_params);
}

void CalibrationViewModel::setThreshold(int value)
{
    m_params.threshold = value;
    m_worker.setParams(m_params);
}

void CalibrationViewModel::pullFrame()
{
    if (const std::optional<QImage> image = m_latest.takeLatest())
        emit previewFrame(*image);
    if (const std::optional<QImage> image = m_latestDetection.takeLatest())
        emit detectionFrame(*image);

    bool locked = false;
    cv::Point centre;
    {
        QMutexLocker locker(&m_lockMutex);
        locked = m_centreLocked;
        centre = m_lockedCentre;
    }
    if (locked) {
        const bool found = m_lastFound.load();
        emit deviationChanged(found, found ? m_lastX.load() - centre.x : 0,
                              found ? m_lastY.load() - centre.y : 0);
    }
}

QString CalibrationViewModel::verdictText(lab6::CalibrationOutcome::Status status)
{
    using Status = lab6::CalibrationOutcome::Status;
    switch (status) {
    case Status::Valid:
        return tr("中心，测量有效");
    case Status::Left:
        return tr("偏左，此次测量无效");
    case Status::Right:
        return tr("偏右，此次测量无效");
    case Status::Divergent:
        return tr("双眼方向不一致，此次测量无效");
    case Status::NoEye:
        return tr("眼点识别失败，此次测量无效");
    case Status::Timeout:
        return tr("采集超时或摄像头不可用，此次测量无效");
    case Status::Idle:
        break;
    }
    return tr("尚未完成中心校准");
}

QString CalibrationViewModel::positionText(const cv::Point &point)
{
    return QStringLiteral("Position: (%1, %2)").arg(point.x).arg(point.y);
}
