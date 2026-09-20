#ifndef LAB3_GLARE_H
#define LAB3_GLARE_H

#include <opencv2/core.hpp>

namespace lab3::glare {
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
