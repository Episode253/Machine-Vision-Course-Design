#include "visual_field_loss.h"

#include <algorithm>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace lab3::visual_field_loss {

// 视野缺失：以画面中心为圆心生成高斯暗斑掩膜，再与原图逐通道取 min 压暗，模拟中心视野缺损；由
// LowVisionViewModel::process() 在工作线程逐帧调用，函数无状态。
cv::Mat apply(const cv::Mat &source, int severity)
{
    // severity<=0 返回原图副本：取消勾选靠参数归零走空操作，而不是在调用处跳过一次处理。
    if (source.empty() || severity <= 0)
        return source.clone();

    // 与其它效果模块一致：先夹到 1~100，再归一化成强度。
    const int bounded = std::clamp(severity, 1, 100);
    const float strength = bounded / 100.0f;
    // 暗斑尺度 sigma 取图像短边的 0.48 倍为基值，保证轻症时暗斑也覆盖视野的大半；
    // strength 越大 sigma 越小（最重时缩到基值的 35%），即「暗斑更集中、中心更暗、可用的周边视野更小」。
    // 下限 12 是防止小图或重症时 sigma 趋近 0，exp() 退化成只压住中心几个像素。
    const float sigma = std::max(12.0f, 0.48f * std::min(source.cols, source.rows)
                                           * (1.0f - 0.65f * strength));
    // 用 32F 单通道承载 [0,1] 的掩膜：浮点避免 8 位量化误差，也便于后面与归一化图像直接取 min。
    cv::Mat mask(source.size(), CV_32F);
    // 中心用 (n-1)*0.5 而不是 n/2：奇偶尺寸下都落在几何中心（n/2 在偶数尺寸上会偏右/偏下半个像素）。
    const float centerX = (source.cols - 1) * 0.5f;
    const float centerY = (source.rows - 1) * 0.5f;
    // 高斯式 exp(-r²/(2σ²)) 的分母，循环外算一次，省掉每像素两次乘法。
    const float denominator = 2.0f * sigma * sigma;
    // 逐像素手写而不是调 GaussianBlur：这里要的是「以中心为峰值的衰减掩膜」，不是归一化的卷积核。
    for (int y = 0; y < mask.rows; ++y) {
        float *row = mask.ptr<float>(y);
        for (int x = 0; x < mask.cols; ++x) {
            const float dx = x - centerX;
            const float dy = y - centerY;
            const float shadow = std::exp(-(dx * dx + dy * dy) / denominator);
            // 中心处 shadow=1 → 掩膜为 1-strength（最暗）；远离中心 shadow→0 → 掩膜→1（保持原亮度）。
            row[x] = 1.0f - strength * shadow;
        }
    }

    // 先归一化到 32F 再运算，避免 8 位下的饱和截断误差。
    cv::Mat sourceFloat;
    source.convertTo(sourceFloat, CV_32F, 1.0 / 255.0);
    // 掩膜是单通道，扩成三通道才能与彩色图逐通道比较。
    cv::Mat maskBgr;
    cv::cvtColor(mask, maskBgr, cv::COLOR_GRAY2BGR);
    // 取 min 即「只压暗、不增亮」：掩膜为 1 的区域像素原样保留。
    cv::min(sourceFloat, maskBgr, sourceFloat);
    cv::Mat result;
    // 还原成原类型（8U）与 0~255 量纲，由 convertTo 做饱和截断。
    sourceFloat.convertTo(result, source.type(), 255.0);
    return result;
}

}
