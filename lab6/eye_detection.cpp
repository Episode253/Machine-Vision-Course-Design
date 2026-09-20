#include "eye_detection.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include <opencv2/imgproc.hpp>

EyeDetectionResult detectDarkEyeTarget(const cv::Mat &frame, const DetectParams &params)
{
    // TODO(lab6): 校验帧与 ROI；为空或越界时填 error 并返回。
    // TODO(lab6): 转灰度并去噪。
    // TODO(lab6): 反转二值化后找最大轮廓。
    // TODO(lab6): 用轮廓包围盒中心作为瞳孔位置（ROI 局部坐标），并置 found。
    EyeDetectionResult result;
    (void)frame;
    (void)params;
    result.error = "not implemented";
    return result;
}

BinocularStatus judgeBinocularOffset(const cv::Point &calibratedLeft, const cv::Point &calibratedRight,
                                     const EyeDetectionResult &actualLeft,
                                     const EyeDetectionResult &actualRight, int tolerance)
{
    if (!actualLeft.found || !actualRight.found)
        return BinocularStatus::DetectionFailed;

    const int leftOffset = actualLeft.center.x - calibratedLeft.x;
    const int rightOffset = actualRight.center.x - calibratedRight.x;
    if (std::abs(leftOffset) < tolerance && std::abs(rightOffset) < tolerance)
        return BinocularStatus::Centered;
    if ((leftOffset <= -tolerance && rightOffset >= tolerance) ||
        (leftOffset >= tolerance && rightOffset <= -tolerance))
        return BinocularStatus::Divergent;
    return (leftOffset <= -tolerance || rightOffset <= -tolerance) ? BinocularStatus::Left
                                                                   : BinocularStatus::Right;
}
