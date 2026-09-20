#include "cataract.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

namespace lab3::cataract {

cv::Mat apply(const cv::Mat &source, int severity)
{
    if (source.empty() || severity <= 0)
        return source.clone();
    const double amount = std::clamp(severity, 0, 100) / 100.0;
    const cv::Scalar warm(38.0, 183.0, 255.0);
    cv::Mat tint(source.size(), source.type(), warm);
    cv::Mat result;
    cv::addWeighted(source, 1.0 - amount, tint, amount, 0.0, result);
    result.convertTo(result, -1, 1.0 - 0.35 * amount, 0.0);
    return result;
}

}
