#include "visual_field_loss.h"

#include <algorithm>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace lab3::visual_field_loss {

cv::Mat apply(const cv::Mat &source, int severity)
{
    if (source.empty() || severity <= 0)
        return source.clone();

    const int bounded = std::clamp(severity, 1, 100);
    const float strength = bounded / 100.0f;
    const float sigma = std::max(12.0f, 0.48f * std::min(source.cols, source.rows)
                                           * (1.0f - 0.65f * strength));
    cv::Mat mask(source.size(), CV_32F);
    const float centerX = (source.cols - 1) * 0.5f;
    const float centerY = (source.rows - 1) * 0.5f;
    const float denominator = 2.0f * sigma * sigma;
    for (int y = 0; y < mask.rows; ++y) {
        float *row = mask.ptr<float>(y);
        for (int x = 0; x < mask.cols; ++x) {
            const float dx = x - centerX;
            const float dy = y - centerY;
            const float shadow = std::exp(-(dx * dx + dy * dy) / denominator);
            row[x] = 1.0f - strength * shadow;
        }
    }

    cv::Mat sourceFloat;
    source.convertTo(sourceFloat, CV_32F, 1.0 / 255.0);
    cv::Mat maskBgr;
    cv::cvtColor(mask, maskBgr, cv::COLOR_GRAY2BGR);
    cv::min(sourceFloat, maskBgr, sourceFloat);
    cv::Mat result;
    sourceFloat.convertTo(result, source.type(), 255.0);
    return result;
}

}
