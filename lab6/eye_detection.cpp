#include "eye_detection.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include <opencv2/imgproc.hpp>

EyeDetectionResult detectDarkEyeTarget(const cv::Mat &frame, const DetectParams &params)
{
    EyeDetectionResult result;
    if (frame.empty()) {
        result.error = "empty frame";
        return result;
    }

    const cv::Rect frameRect(0, 0, frame.cols, frame.rows);
    const cv::Rect roi = params.roi & frameRect;
    if (roi.empty()) {
        result.error = "eye ROI is outside the frame";
        return result;
    }

    cv::Mat gray;
    if (frame.channels() == 1)
        gray = frame(roi).clone();
    else if (frame.channels() == 3)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGR2GRAY);
    else if (frame.channels() == 4)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGRA2GRAY);
    else {
        result.error = "unsupported frame channels";
        return result;
    }

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0.0);
    cv::threshold(gray, result.binary, std::clamp(params.threshold, 0, 255), 255,
                  cv::THRESH_BINARY_INV);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(result.binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        result.error = "no dark pupil candidate";
        return result;
    }

    const auto largest = std::max_element(contours.begin(), contours.end(),
                                           [](const auto &left, const auto &right) {
                                               return cv::contourArea(left) < cv::contourArea(right);
                                           });
    const cv::Rect bounds = cv::boundingRect(*largest);
    if (bounds.area() <= 0) {
        result.error = "invalid pupil candidate";
        return result;
    }
    result.center = bounds.tl() + cv::Point(bounds.width / 2, bounds.height / 2);
    result.found = true;
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
