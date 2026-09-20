#ifndef LAB3_BLUR_TWO_H
#define LAB3_BLUR_TWO_H

#include <opencv2/core.hpp>

namespace lab3::blur_two {
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
