#include "eye_detection.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include <opencv2/imgproc.hpp>

// 在给定 ROI 内检出瞳孔：灰度化 → 去噪 → 反转二值化 → 取最大轮廓 → 包围盒中心。
// 跑在采集线程（预览）或测量线程，只读入参，不保留跨帧状态。
EyeDetectionResult detectDarkEyeTarget(const cv::Mat &frame, const DetectParams &params)
{
    EyeDetectionResult result;
    if (frame.empty()) {
        // 空帧连行列数都没有，无法做 ROI 求交，直接失败。
        result.error = "empty frame";
        return result;
    }

    // ROI 来自界面滑块（默认上限 1280×960），未必落在当前画面范围内；
    // 先与画面矩形求交再使用，保证后面的子矩阵访问不越界。
    const cv::Rect frameRect(0, 0, frame.cols, frame.rows);
    const cv::Rect roi = params.roi & frameRect;
    if (roi.empty()) {
        // 求交为空说明 ROI 完全在画面之外，属于参数配置问题而非“没找到瞳孔”，分开报错。
        result.error = "eye ROI is outside the frame";
        return result;
    }

    // 只在 ROI 子图上转换，不必处理整帧；按通道数分流，灰度/彩色/带 alpha 都有对应路径。
    cv::Mat gray;
    if (frame.channels() == 1)
        // 单通道时 frame(roi) 只是原帧的视图，必须 clone：
        // 下一步的模糊是就地写入，否则会改到调用方的帧数据上。
        gray = frame(roi).clone();
    else if (frame.channels() == 3)
        // cvtColor 会自行分配输出，无需再 clone。
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGR2GRAY);
    else if (frame.channels() == 4)
        cv::cvtColor(frame(roi), gray, cv::COLOR_BGRA2GRAY);
    else {
        result.error = "unsupported frame channels";
        return result;
    }

    // 先平滑再二值化：抑制传感器噪声，以及睫毛、反光造成的孤立亮点，
    // 否则会碎出大量小轮廓，最大轮廓可能落在噪声上。sigma 传 0 表示由核大小自动推算。
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0.0);
    // 瞳孔比周围皮肤、巩膜暗，用 INV 反转后暗区变成白色前景，而 findContours 找的正是白前景，
    // 所以这里必须用 THRESH_BINARY_INV。阈值钳到 0–255，防滑块越界值。
    cv::threshold(gray, result.binary, std::clamp(params.threshold, 0, 255), 255,
                  cv::THRESH_BINARY_INV);

    // RETR_EXTERNAL 只取最外层轮廓，不关心孔洞层级；CHAIN_APPROX_SIMPLE 只保留拐点以压缩点数。
    // findContours 会改动输入图，这里直接让它写 result.binary：省一份拷贝，
    // 也正好把二值图留给界面回显。
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(result.binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) {
        // ROI 内没有暗区：阈值过低，或这一段画面整体过亮。
        result.error = "no dark pupil candidate";
        return result;
    }

    // 瞳孔是 ROI 内面积最大的暗区，而眼睑阴影、睫毛等只会是细长小块，
    // 所以按面积取最大轮廓比按位置或形状筛选更稳定。
    const auto largest = std::max_element(contours.begin(), contours.end(),
                                           [](const auto &left, const auto &right) {
                                               return cv::contourArea(left) < cv::contourArea(right);
                                           });
    // 用包围盒而不是轮廓矩心：轮廓被遮挡缺角时矩心会偏移，包围盒中心更稳。
    const cv::Rect bounds = cv::boundingRect(*largest);
    if (bounds.area() <= 0) {
        // 退化包围盒（宽或高为 0）的防御性检查，正常路径不会命中。
        result.error = "invalid pupil candidate";
        return result;
    }
    // contours 与 bounds 都在 ROI 子图坐标系内，因此这里得到的是 ROI 局部坐标，
    // 要对应整帧位置需由调用方自行加上 ROI 原点。
    result.center = bounds.tl() + cv::Point(bounds.width / 2, bounds.height / 2);
    result.found = true;
    return result;
}

// 双眼判定：只看水平偏移（注视点横移是主要分量，纵向不参与比较），容差默认 4px 由头文件给出。
// 纯函数、无状态，可在任意线程调用。
BinocularStatus judgeBinocularOffset(const cv::Point &calibratedLeft, const cv::Point &calibratedRight,
                                     const EyeDetectionResult &actualLeft,
                                     const EyeDetectionResult &actualRight, int tolerance)
{
    // 任一眼没检出就无法成对比较，直接判失败，不再看另一只眼。
    if (!actualLeft.found || !actualRight.found)
        return BinocularStatus::DetectionFailed;

    // 偏移 = 当前中心 - 标定中心：负值向左，正值向右。
    const int leftOffset = actualLeft.center.x - calibratedLeft.x;
    const int rightOffset = actualRight.center.x - calibratedRight.x;
    // 两眼都在容差内 → 中心。注意用的是严格小于，偏移正好等于容差算超差。
    if (std::abs(leftOffset) < tolerance && std::abs(rightOffset) < tolerance)
        return BinocularStatus::Centered;
    // 两眼反向偏移（一左一右）→ 发散：两眼没有看同一个方向。
    if ((leftOffset <= -tolerance && rightOffset >= tolerance) ||
        (leftOffset >= tolerance && rightOffset <= -tolerance))
        return BinocularStatus::Divergent;
    // 剩下的都是同向偏移，任一眼偏左即判 Left，否则 Right。
    return (leftOffset <= -tolerance || rightOffset <= -tolerance) ? BinocularStatus::Left
                                                                   : BinocularStatus::Right;
}
