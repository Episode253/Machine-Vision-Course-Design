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



// 标注配色，注意 OpenCV 是 BGR 顺序：ROI 框绿、瞳孔红、锁定中心橙、偏差连线与文字青。
const cv::Scalar kRoiColour(60, 200, 60);
const cv::Scalar kPupilColour(0, 0, 255);
const cv::Scalar kCentreColour(255, 160, 0);
const cv::Scalar kDeviationColour(0, 220, 220);

// 在画布上画注视标注：未锁定中心时只画瞳孔十字，锁定后加画基准中心的斜十字以及
// 基准到瞳孔的连线（偏移方向和大小一眼可见）。两个点必须已经是同一张图的坐标系。
void drawFixationOverlay(cv::Mat &canvas, const cv::Point &pupil, bool found,
                         const cv::Point &centre, bool locked)
{
    if (locked) {
        // 斜十字与瞳孔的正十字形状不同，两者靠得很近时也不会看混。
        cv::drawMarker(canvas, centre, kCentreColour, cv::MARKER_TILTED_CROSS, 14, 2);
        if (found)
            cv::line(canvas, centre, pupil, kDeviationColour, 2);
    }
    // 未检出瞳孔就不画，避免把别的东西错当成当前瞳孔位置。
    if (found)
        cv::drawMarker(canvas, pupil, kPupilColour, cv::MARKER_CROSS, 12, 2);
}


// 给彩色预览帧加上 ROI 框、瞳孔与偏差标注，再转成 QImage。
// 在预览线程内调用（帧回调里），只画自己 clone 出来的图，最后经 toQImage 交给界面。
QImage annotatePreview(const cv::Mat &frame, const cv::Rect &roi,
                       const EyeDetectionResult &detection, bool locked,
                       const cv::Point &lockedCentre)
{
    // clone 后再画：入参是共享的只读帧，不能就地改。
    cv::Mat canvas = frame.clone();
    if (canvas.channels() != 3)
        // 只有三通道能直接画彩色标注，其它格式原样返回，至少保证界面有画面。
        return labqt::toQImage(canvas);

    // 这里再求一次交：ROI 来自界面滑块，可能超出实际画面，矩形越界会让绘制函数出错。
    const cv::Rect clipped = roi & cv::Rect(0, 0, canvas.cols, canvas.rows);
    if (clipped.empty())
        return labqt::toQImage(canvas);

    cv::rectangle(canvas, clipped, kRoiColour, 2);

    // detection.center 与 lockedCentre 都是 ROI 局部坐标，加上 clipped 的原点才落到画布坐标。
    drawFixationOverlay(canvas, clipped.tl() + detection.center, detection.found,
                        clipped.tl() + lockedCentre, locked);
    if (locked && detection.found) {
        // 在 ROI 左上角写一行偏移量：操作者不用估读十字，直接看数值。
        char text[40];
        std::snprintf(text, sizeof(text), "dx=%+d dy=%+d",
                      detection.center.x - lockedCentre.x, detection.center.y - lockedCentre.y);
        cv::putText(canvas, text, clipped.tl() + cv::Point(4, 16), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    kDeviationColour, 1, cv::LINE_AA);
    }
    return labqt::toQImage(canvas);
}



// 把二值图加上标注后转成 QImage；先转 BGR 才能在上面画彩色标记。
// 预览线程与测量线程都会调用它，只依赖入参、不访问共享状态，所以是线程安全的。
QImage annotateDetection(const EyeDetectionResult &detection, bool locked,
                         const cv::Point &lockedCentre)
{
    if (detection.binary.empty())
        // 连二值化都没走到（ROI 越界等），返回空 QImage，界面侧跳过这次刷新。
        return QImage();
    cv::Mat canvas;
    cv::cvtColor(detection.binary, canvas, cv::COLOR_GRAY2BGR);
    // 二值图里的坐标就是 ROI 局部坐标，与 detection.center 同一坐标系，无需换算。
    drawFixationOverlay(canvas, detection.center, detection.found, lockedCentre, locked);
    if (locked) {
        // 检出且在 4px 内画绿框，否则红框——整幅边框颜色就是“是否居中”的快速提示。
        // 这里的 4px 是硬编码，与 CalibrationSession 的默认容差取同一个值。
        const bool centred = detection.found
            && std::abs(detection.center.x - lockedCentre.x) < 4
            && std::abs(detection.center.y - lockedCentre.y) < 4;
        cv::rectangle(canvas, cv::Rect(0, 0, canvas.cols, canvas.rows),
                      centred ? kRoiColour : kPupilColour, 2);
    }
    return labqt::toQImage(canvas);
}

}

// 构造在 GUI 线程：只建定时器，不碰摄像头（设备要等 startCalibration 才打开）。
CalibrationViewModel::CalibrationViewModel(QObject *parent)
    : QObject(parent)
{
    m_frameTimer = new QTimer(this);
    // 33ms ≈ 30Hz，与预览线程 20ms 的节拍错开，靠“只留最新帧”的槽自动丢弃多余帧。
    m_frameTimer->setInterval(33);
    connect(m_frameTimer, &QTimer::timeout, this, &CalibrationViewModel::pullFrame);
}

// 析构顺序就是停机顺序：停预览线程 → 停定时器 → 等测量线程收尾 → 关摄像头。
// 倒过来的话，测量线程可能在后两者释放后还在取帧。
CalibrationViewModel::~CalibrationViewModel()
{
    // stopPreview 会阻塞到预览线程真正退出。
    m_worker.stopPreview();
    m_frameTimer->stop();

    // 测量线程可能正卡在 measureOnce 的等帧循环里，join 让它跑完这一轮再退出。
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

// 启动校准：清状态 → 开摄像头 → 起预览线程 → 开刷新定时器。GUI 线程调用。
void CalibrationViewModel::startCalibration()
{
    // 校准阶段持续取帧并累积有效瞳孔位置；确认后才锁定中心点供后续偏差判断使用。
    // 已在预览中就直接返回，重复点按钮不会起第二个线程。
    if (m_worker.previewRunning())
        return;



    // 先等上一次的测量线程结束：预览与测量共用同一个 FrameSource，两者不能同时取帧。
    m_measureWorker.join();

    std::string error;
    if (!m_worker.open(&error)) {
        emit errorOccurred(QString::fromStdString(error));
        return;
    }

    // 新一轮校准：清掉累积均值与上一次锁定的中心，最近一帧的位置也一并复位。
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

    // 把当前参数（ROI、阈值）交给 worker，再启动预览；回调里的代码跑在预览线程。
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


            // 始终记录最近一帧的检出情况：confirmCentre 优先用它作为锁定中心。
            m_lastFound.store(detection.found);
            if (detection.found) {
                m_lastX.store(detection.center.x);
                m_lastY.store(detection.center.y);
            }
            // 未锁定中心前持续累积样本；锁定后不再更新均值，避免基准点被后续帧带偏。
            if (!locked) {
                QMutexLocker locker(&m_sessionMutex);
                m_session.observe(detection);
            }
            // 标注要画在整帧画布上，所以取当前 ROI 参数一起交给绘制函数。
            const cv::Rect roi = m_worker.params().roi;
            m_latest.set(annotatePreview(frame->mat, roi, detection, locked, lockedCentre));
            m_latestDetection.set(annotateDetection(detection, locked, lockedCentre));
        });
    if (!started) {
        emit errorOccurred(tr("摄像头预览启动失败"));
        return;
    }
    // 预览线程已在产帧，这时才开刷新定时器并通知界面切到“预览中”状态。
    m_frameTimer->start();
    emit previewActiveChanged(true);
    qCInfo(labCamera) << "calibration preview started";
}

// 锁定基准中心：只在有瞳孔位置可取时才允许，锁定后界面转入实时偏差显示。
// 注意 confirmCentre 只是“取一个基准点”，预览仍在继续，不做停机动作。
void CalibrationViewModel::confirmCentre()
{
    // 没在预览就没有瞳孔位置可用。
    if (!m_worker.previewRunning()) {
        emit errorOccurred(tr("请先开始校准"));
        return;
    }

    // 优先取最近一帧检出的位置：它对应操作者按下按钮那一刻的注视点；
    // 当前帧没检出时退回校准期的累积均值（对单帧噪声更稳）。
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
    // 锁定：从此预览回调不再更新累积均值，判定改用这个固定基准点。
    {
        QMutexLocker locker(&m_lockMutex);
        m_centreLocked = true;
        m_lockedCentre = centre;
    }

    emit calibrationFinished(positionText(centre));
    qCInfo(labCamera) << "centre locked at" << centre.x << centre.y;
}

// 停止预览：停线程 → 停定时器 → 清掉画面。锁定过的中心不受影响，测量仍可用它做基准。
void CalibrationViewModel::stopCalibration()
{
    // 本来就没在预览：清显示即可，不必动线程。
    if (!m_worker.previewRunning()) {
        m_latest.clear();
        m_latestDetection.clear();
        return;
    }
    // 先停预览线程再停定时器，最后清槽，避免停止后画面还闪一下。
    m_worker.stopPreview();
    m_frameTimer->stop();

    m_latest.clear();
    m_latestDetection.clear();
    emit previewActiveChanged(false);
    qCInfo(labCamera) << "calibration preview stopped";
}

// 单次测量：必须在已打开摄像头且已锁定中心之后调用；测量在独立线程里跑，界面不阻塞。
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
    // 测量是标定之后的动作，没有锁定中心就没有比较基准。
    if (!locked) {
        emit errorOccurred(tr("请先确认中心"));
        return;
    }
    // 预览与测量都要从同一个 FrameSource 取帧，先停掉预览。
    stopCalibration();

    // 这个 lambda 跑在 m_measureWorker 线程里：measureOnce 最长阻塞 2s，放 GUI 线程会卡住界面；
    // 末尾的 emit 靠 Qt 队列连接投递回 GUI 线程。
    m_measureWorker.start([this] {
        lab6::CalibrationOutcome outcome;
        // 2s 超时窗口：期间反复取帧，拿到第一帧成功检测就提前返回，不等满窗口。
        const std::optional<EyeDetectionResult> detection =
            m_worker.measureOnce(std::chrono::milliseconds(2000));
        // 取锁定中心的副本：后面既用它做判定基准，也用它画标注。
        cv::Point lockedCentre;
        {
            QMutexLocker locker(&m_lockMutex);
            lockedCentre = m_lockedCentre;
        }
        if (!detection) {
            // 没有结果只可能是超时、被请求停止或摄像头不可用，统一记为 Timeout。
            outcome.status = lab6::CalibrationOutcome::Status::Timeout;
        } else {
            // 用锁定的中心（而不是累积均值）作基准，所以走 evaluateAgainst。
            {
                QMutexLocker locker(&m_sessionMutex);
                outcome = m_session.evaluateAgainst(*detection, lockedCentre);
            }

            // 把这次检测的二值图也发一份给界面，便于对照阈值效果与判定结果。
            emit detectionFrame(annotateDetection(*detection, true, lockedCentre));
        }
        emit measurementFinished(positionText(outcome.centre), positionText(outcome.actual),
                                 verdictText(outcome.status), outcome.valid());
    });
}

// 设置 ROI：入参是两个对角点，先取 min 归一成左上角、再用差值的绝对值算宽高，
// 所以拖滑块的顺序不影响结果（x1 可以大于 x2）。
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

// 33ms 定时器槽（GUI 线程）：把预览线程放进槽的最新画面取走并 emit 给界面；
// takeLatest 取走即空，没有新帧的这一轮什么都不发，所以这里天然是“有新才刷”。
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
    // 只有锁定中心之后才有“偏差”可言；未锁定时界面只显示画面，不显示偏差。
    if (locked) {
        const bool found = m_lastFound.load();
        emit deviationChanged(found, found ? m_lastX.load() - centre.x : 0,
                              found ? m_lastY.load() - centre.y : 0);
    }
}

// 把判定状态翻成界面文案：有效与各种无效都直说，避免操作者把无效测量当成有效结果。
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
    // Divergent 是枚举里的双眼状态，单眼测量路径不会命中，这里保留对应文案。
    case Status::Divergent:
        return tr("双眼方向不一致，此次测量无效");
    case Status::NoEye:
        return tr("眼点识别失败，此次测量无效");
    case Status::Timeout:
        return tr("采集超时或摄像头不可用，此次测量无效");
    case Status::Idle:
        // Idle 不复用上面的文案，落到函数末尾统一返回“未校准”。
        break;
    }
    return tr("尚未完成中心校准");
}

// 坐标文案的统一格式，中心点与实测位置共用，保证界面两处显示风格一致。
QString CalibrationViewModel::positionText(const cv::Point &point)
{
    return QStringLiteral("Position: (%1, %2)").arg(point.x).arg(point.y);
}
