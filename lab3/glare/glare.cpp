#include "glare.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

namespace lab3::glare {

// 眩光：在缩略图上提取高亮区、模糊成泛光，再按该处原图颜色加性叠回，模拟强光在眼内的散射；
// 由 LowVisionViewModel::process() 在工作线程逐帧调用，函数无状态。
cv::Mat apply(const cv::Mat &source, int severity)
{
    // severity<=0 返回原图副本：取消勾选靠参数归零走空操作，而不是在调用处跳过一次处理。
    if (source.empty() || severity <= 0)
        return source.clone();

    const int bounded = std::clamp(severity, 1, 100);
    cv::Mat small;
    // 先降到 1/4 分辨率再处理：同样大小的模糊核在这里等效覆盖 4 倍范围，开销却降到约 1/16；INTER_AREA 抗混叠。
    cv::resize(source, small, cv::Size(), 0.25, 0.25, cv::INTER_AREA);
    cv::Mat smooth;
    // 在缩略图上先做 5x5 均值平滑，避免单个过亮噪点就把整块区域判成高光。
    cv::blur(small, smooth, cv::Size(5, 5));
    cv::Mat gray;
    cv::cvtColor(smooth, gray, cv::COLOR_BGR2GRAY);
    cv::Mat bright;
    // 阈值随 severity 降低（sev=1 时 165，sev=100 时 132）：病情越重，越多区域被判为亮区，泛光范围越大。
    cv::threshold(gray, bright, 165 - bounded / 3, 255, cv::THRESH_BINARY);
    // Size(0,0) 让 OpenCV 由 sigma 反推核大小；sigma 3→11 决定泛光的扩散范围（在 1/4 分辨率上，等效原图的 4 倍）。
    cv::GaussianBlur(bright, bright, cv::Size(0, 0), 3.0 + bounded * 0.08);
    cv::Mat glowSmall;
    // 泛光颜色取自平滑后的原图而不是纯白：高光不会变成死白，而是带该处色彩的光晕。
    smooth.convertTo(glowSmall, CV_32F, 1.0 / 255.0);
    cv::Mat brightFloat;
    // 阈值图归一化后乘增益（上限 0.8）：severity 越高泛光越亮，封顶是为了避免整幅过曝、细节丢失。
    bright.convertTo(brightFloat, CV_32F, (bounded / 100.0) * 0.8 / 255.0);
    cv::Mat brightBgr;
    cv::cvtColor(brightFloat, brightBgr, cv::COLOR_GRAY2BGR);
    cv::Mat glow;
    // 用亮区掩膜调制原图颜色：泛光只出现在亮处，且带上该处的色调。
    cv::multiply(glowSmall, brightBgr, glow);
    // 上采样回原尺寸；线性插值保证缩小再放大后的泛光边缘平滑。
    cv::resize(glow, glow, source.size(), 0, 0, cv::INTER_LINEAR);
    cv::Mat sourceFloat;
    source.convertTo(sourceFloat, CV_32F);
    const cv::Mat &glowBgr = glow;
    // 加性混合而不是覆盖：glowBgr*255 生成临时矩阵后与原图相加，即「在原图上叠亮」。
    cv::add(sourceFloat, glowBgr * 255.0, sourceFloat);
    cv::Mat result;
    // 转回原类型时自动饱和，避免加亮溢出后回绕成暗点。
    sourceFloat.convertTo(result, source.type());
    return result;
}

}
