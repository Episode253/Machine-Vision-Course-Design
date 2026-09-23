#ifndef LAB3_VISUAL_FIELD_LOSS_H
#define LAB3_VISUAL_FIELD_LOSS_H

#include <opencv2/core.hpp>

namespace lab3::visual_field_loss {
// 在图上叠加中心高斯暗斑并返回新矩阵；severity<=0 时返回原图副本。纯函数、无共享状态，由工作线程调用。
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
