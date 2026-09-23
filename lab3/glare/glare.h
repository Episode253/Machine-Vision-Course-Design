#ifndef LAB3_GLARE_H
#define LAB3_GLARE_H

#include <opencv2/core.hpp>

namespace lab3::glare {
// 在亮区叠加加性泛光并返回新矩阵；severity<=0 时返回原图副本。纯函数、无共享状态，由工作线程调用。
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
