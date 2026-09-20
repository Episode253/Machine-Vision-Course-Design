#include "glare.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

namespace lab3::glare {

cv::Mat apply(const cv::Mat &source, int severity)
{
    if (source.empty() || severity <= 0)
        return source.clone();

    const int bounded = std::clamp(severity, 1, 100);
    cv::Mat small;
    cv::resize(source, small, cv::Size(), 0.25, 0.25, cv::INTER_AREA);
    cv::Mat smooth;
    cv::blur(small, smooth, cv::Size(5, 5));
    cv::Mat gray;
    cv::cvtColor(smooth, gray, cv::COLOR_BGR2GRAY);
    cv::Mat bright;
    cv::threshold(gray, bright, 165 - bounded / 3, 255, cv::THRESH_BINARY);
    cv::GaussianBlur(bright, bright, cv::Size(0, 0), 3.0 + bounded * 0.08);
    cv::Mat glowSmall;
    smooth.convertTo(glowSmall, CV_32F, 1.0 / 255.0);
    cv::Mat brightFloat;
    bright.convertTo(brightFloat, CV_32F, (bounded / 100.0) * 0.8 / 255.0);
    cv::Mat brightBgr;
    cv::cvtColor(brightFloat, brightBgr, cv::COLOR_GRAY2BGR);
    cv::Mat glow;
    cv::multiply(glowSmall, brightBgr, glow);
    cv::resize(glow, glow, source.size(), 0, 0, cv::INTER_LINEAR);
    cv::Mat sourceFloat;
    source.convertTo(sourceFloat, CV_32F);
    const cv::Mat &glowBgr = glow;
    cv::add(sourceFloat, glowBgr * 255.0, sourceFloat);
    cv::Mat result;
    sourceFloat.convertTo(result, source.type());
    return result;
}

}
