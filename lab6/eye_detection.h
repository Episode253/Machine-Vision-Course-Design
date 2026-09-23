#ifndef LAB6_EYE_DETECTION_H
#define LAB6_EYE_DETECTION_H

#include <string>

#include <opencv2/core.hpp>

// 单帧瞳孔检测结果，同时兼作失败信息载体。
// found 为真时 center 与 binary 有效；为假时 error 说明失败原因。
struct EyeDetectionResult {
    bool found = false;
    // 瞳孔中心，ROI 局部坐标（相对 ROI 左上角）；映射回整帧位置需由调用方加上 ROI 原点。
    cv::Point center{};
    // 反转二值化后的图，供界面回显与调参观察；连二值化都没走到时为空。
    cv::Mat binary;
    std::string error;
};

// 检测参数：界面滑块写入，检测时取一份快照读取。
struct DetectParams {
    // 待搜索的眼部区域，整帧绝对像素坐标；允许越界，检测前会与画面矩形求交。
    cv::Rect roi{0, 0, 50, 50};
    // 二值化阈值：灰度低于它的像素判为暗区（瞳孔候选），默认 55。
    int threshold = 55;
};

// 在 ROI 内检出瞳孔：取 ROI 内面积最大的暗色连通块，以其包围盒中心作为瞳孔位置（ROI 局部坐标）。
// 无内部状态、只读入参，由 MeasurementWorker::detect() 在预览线程或测量线程调用。
EyeDetectionResult detectDarkEyeTarget(const cv::Mat &frame, const DetectParams &params);

// 双眼相对各自标定位置的水平偏移判定结果。
enum class BinocularStatus { Centered, Left, Right, Divergent, DetectionFailed };

// 双眼水平偏移判定：两眼都检出时比较各自与标定位置的水平偏移（默认容差 4px）。
// 供双眼判定场景使用；lab6 界面当前的测量路径只判单眼（CalibrationSession::evaluateAgainst），
// 并未调用本函数。
BinocularStatus judgeBinocularOffset(const cv::Point &calibratedLeft, const cv::Point &calibratedRight,
                                     const EyeDetectionResult &actualLeft,
                                     const EyeDetectionResult &actualRight, int tolerance = 4);

#endif
