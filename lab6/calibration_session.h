#ifndef LAB6_CALIBRATION_SESSION_H
#define LAB6_CALIBRATION_SESSION_H

#include <cstdint>

#include <opencv2/core.hpp>

#include "eye_detection.h"

namespace lab6 {



// 单次测量的判定结果：status 是结论，centre/actual 是基准点与本次实测瞳孔位置（均为整帧坐标），
// offsetX 是 actual 相对 centre 的水平偏移。
struct CalibrationOutcome {
    // Idle 尚未校准（没有基准点），Valid 在容差内，Left/Right 超容差并指明方向，
    // NoEye 本帧未检出瞳孔，Timeout 采集超时；
    // Divergent 是枚举里的双眼状态，单眼判定路径不会产生它。
    enum class Status { Idle, Valid, Left, Right, Divergent, NoEye, Timeout };

    Status status = Status::Idle;
    // 基准中心点，由 evaluateAgainst 的调用方给定（已锁定的中心或累积均值）。
    cv::Point centre{};
    // 本次检出的瞳孔位置；NoEye 等失败状态下保持默认值。
    cv::Point actual{};
    int offsetX = 0;

    // 便捷判断，供界面直接问“这次测量有效吗”。
    bool valid() const { return status == Status::Valid; }
};



// 标定会话：把校准期每一帧检出的瞳孔位置累加成均值，作为后续偏差判定的基准点。
// 自身不加锁，由调用方（CalibrationViewModel 的 m_sessionMutex）保证互斥。
class CalibrationSession {
public:
    void reset();

    void observe(const EyeDetectionResult &detection);
    bool calibrated() const { return m_samples > 0; }
    cv::Point centre() const { return m_centre; }
    std::uint32_t samples() const { return m_samples; }

    // 以累积均值为基准点判定。
    CalibrationOutcome evaluate(const EyeDetectionResult &actual, int tolerance = 4) const;



    // 用调用方显式给定的中心作为基准点判定：确认中心后，基准点是锁定的那一个点，
    // 不一定等于累积均值，所以判定要支持传入基准点。
    CalibrationOutcome evaluateAgainst(const EyeDetectionResult &actual, const cv::Point &centre,
                                       int tolerance = 4) const;

private:
    // 累积均值与已积累的样本数：样本数为 0 表示还没校准（calibrated() 为假）。
    cv::Point m_centre{};
    std::uint32_t m_samples = 0;
};

}

#endif
