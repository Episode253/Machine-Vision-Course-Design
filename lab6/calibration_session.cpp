#include "calibration_session.h"

#include <cstdlib>

namespace lab6 {

// 清空累积，回到未校准状态；在 CalibrationViewModel::startCalibration 开始新一轮校准时调用。
void CalibrationSession::reset()
{
    // 归零后 calibrated() 为假，判定会退回 Idle。
    m_centre = cv::Point();
    m_samples = 0;
}

// 累加一帧的检测结果，逐个样本地更新均值；由预览线程在每帧回调里调用（调用方持锁）。
void CalibrationSession::observe(const EyeDetectionResult &detection)
{
    // 未检出瞳孔的帧不参与平均，否则会把均值往 (0,0) 拖。
    if (!detection.found)
        return;

    // 增量均值：centre = (centre * n + p) / (n + 1)，不必保存全部样本；
    // 首帧时 n 为 0，均值直接取该帧位置。乘法用 long long 做中间结果以防溢出。
    m_centre.x = static_cast<int>((static_cast<long long>(m_centre.x) * m_samples
                                   + detection.center.x) / (m_samples + 1));
    m_centre.y = static_cast<int>((static_cast<long long>(m_centre.y) * m_samples
                                   + detection.center.y) / (m_samples + 1));
    ++m_samples;
}

// 以累积均值为基准判定；界面测量走的是 evaluateAgainst（显式传锁定中心），本函数是它的简化入口。
CalibrationOutcome CalibrationSession::evaluate(const EyeDetectionResult &actual,
                                                int tolerance) const
{
    return evaluateAgainst(actual, m_centre, tolerance);
}

// 判定实测瞳孔位置相对给定基准点的水平偏移；
// 由 CalibrationViewModel::measure 在测量线程内调用（调用方持 m_sessionMutex）。
CalibrationOutcome CalibrationSession::evaluateAgainst(const EyeDetectionResult &actual,
                                                       const cv::Point &centre, int tolerance) const
{
    CalibrationOutcome outcome;
    // 无论判定结果如何都把基准点回填，界面在失败时也能照常显示“校准中心”。
    outcome.centre = centre;
    if (m_samples == 0) {
        // 一个有效样本都没有 → 还没校准，没有可比较的基准。
        outcome.status = CalibrationOutcome::Status::Idle;
        return outcome;
    }
    if (!actual.found) {
        // 本帧没找到瞳孔与“偏移超容差”是两回事，单独区分开，避免误导操作者。
        outcome.status = CalibrationOutcome::Status::NoEye;
        return outcome;
    }
    outcome.actual = actual.center;
    outcome.offsetX = actual.center.x - centre.x;
    // 只看水平偏移：容差内为中心，否则按符号分偏左/偏右。
    // 比较用严格小于，偏移正好等于容差算超差。
    if (std::abs(outcome.offsetX) < tolerance)
        outcome.status = CalibrationOutcome::Status::Valid;
    else
        outcome.status = outcome.offsetX < 0 ? CalibrationOutcome::Status::Left
                                             : CalibrationOutcome::Status::Right;
    return outcome;
}

}
