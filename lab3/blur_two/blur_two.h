#ifndef LAB3_BLUR_TWO_H
#define LAB3_BLUR_TWO_H

#include <opencv2/core.hpp>

namespace lab3::blur_two {
// 对输入图施加高斯模糊并返回新矩阵；severity<=0 时返回原图副本。纯函数、无共享状态，由工作线程调用。
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
