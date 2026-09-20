#ifndef LAB3_CATARACT_H
#define LAB3_CATARACT_H

#include <opencv2/core.hpp>

namespace lab3::cataract {
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
