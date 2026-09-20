#include "blur_two.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

namespace lab3::blur_two {

cv::Mat apply(const cv::Mat &source, int severity)
{
    if (source.empty() || severity <= 0)
        return source.clone();
    const int bounded = std::clamp(severity, 1, 100);
    const int kernel = 1 + 2 * ((bounded + 4) / 5);
    const double sigma = 0.4 + bounded * 0.08;
    cv::Mat result;
    cv::GaussianBlur(source, result, cv::Size(kernel, kernel), sigma, sigma);
    return result;
}

}
