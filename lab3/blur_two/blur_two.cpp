#include "blur_two.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

namespace lab3::blur_two {

// 视力模糊：按 severity（0~100）做整幅高斯模糊；由 LowVisionViewModel::process() 在工作线程逐帧调用，函数无状态。
cv::Mat apply(const cv::Mat &source, int severity)
{
    // severity<=0 时返回原图副本，这就是「取消勾选」的实现方式：参数归零即空操作，调用处不必做分支跳过。
    if (source.empty() || severity <= 0)
        return source.clone();
    // 滑条量程本来就是 0~100，这里再夹一次是防御越界调用；下界取 1 是因为 0 已在上一分支返回。
    const int bounded = std::clamp(severity, 1, 100);
    // 核边长 = 1 + 2*((sev+4)/5)：先加 4 让整数除法变成向上取整，再乘 2 加 1 保证恒为奇数（GaussianBlur 要求）；
    // sev 每涨 5 级换一档核，1→3、100→41。
    const int kernel = 1 + 2 * ((bounded + 4) / 5);
    // sigma 随 severity 线性增长（0.48→8.4），模糊程度因此平滑连续，不像核边长那样一档一档跳。
    const double sigma = 0.4 + bounded * 0.08;
    cv::Mat result;
    // sigma 同时给 x/y 两个方向，得到各向同性模糊；核边长与 sigma 一起给，具体系数由 OpenCV 生成。
    cv::GaussianBlur(source, result, cv::Size(kernel, kernel), sigma, sigma);
    return result;
}

}
