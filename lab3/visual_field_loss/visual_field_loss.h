#ifndef LAB3_VISUAL_FIELD_LOSS_H
#define LAB3_VISUAL_FIELD_LOSS_H

#include <opencv2/core.hpp>

namespace lab3::visual_field_loss {
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
