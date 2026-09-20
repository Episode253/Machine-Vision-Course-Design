#ifndef LAB6_EYE_DETECTION_H
#define LAB6_EYE_DETECTION_H

#include <string>

#include <opencv2/core.hpp>




struct EyeDetectionResult {
    bool found = false;
    cv::Point center{};
    cv::Mat binary;
    std::string error;
};

struct DetectParams {
    cv::Rect roi{0, 0, 50, 50};
    int threshold = 55;
};








EyeDetectionResult detectDarkEyeTarget(const cv::Mat &frame, const DetectParams &params);

enum class BinocularStatus { Centered, Left, Right, Divergent, DetectionFailed };


BinocularStatus judgeBinocularOffset(const cv::Point &calibratedLeft, const cv::Point &calibratedRight,
                                     const EyeDetectionResult &actualLeft,
                                     const EyeDetectionResult &actualRight, int tolerance = 4);

#endif
