#ifndef LAB3_CATARACT_H
#define LAB3_CATARACT_H

#include <opencv2/core.hpp>

namespace lab3::cataract {
// 对图施加暖色偏色与降增益并返回新矩阵；severity<=0 时返回原图副本。纯函数、无共享状态，由工作线程调用。
cv::Mat apply(const cv::Mat &source, int severity);
}

#endif
