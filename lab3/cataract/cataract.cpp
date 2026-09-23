#include "cataract.h"

#include <algorithm>

#include <opencv2/imgproc.hpp>

namespace lab3::cataract {

// 白内障：向暖色（琥珀）线性混色再整体降增益，模拟晶状体混浊造成的黄褐化与透光率下降；
// 由 LowVisionViewModel::process() 在工作线程逐帧调用，函数无状态。
cv::Mat apply(const cv::Mat &source, int severity)
{
    // severity<=0 返回原图副本：取消勾选靠参数归零走空操作，而不是在调用处跳过一次处理。
    if (source.empty() || severity <= 0)
        return source.clone();
    // 归一化成 0~1 的混色比例。这里下界写 0（其余模块写 1），但 severity<=0 已在上面返回，实际仍是 (0,1]。
    const double amount = std::clamp(severity, 0, 100) / 100.0;
    // BGR 顺序的琥珀色：蓝最低、红最高，对应晶状体混浊偏黄褐的色调。
    const cv::Scalar warm(38.0, 183.0, 255.0);
    // 构造同尺寸同类型的纯色底图供 addWeighted 使用；amount 越大，结果越接近纯暖色。
    cv::Mat tint(source.size(), source.type(), warm);
    cv::Mat result;
    cv::addWeighted(source, 1.0 - amount, tint, amount, 0.0, result);
    // rtype=-1 表示保持原类型，乘上 1-0.35*amount 模拟透光率下降：最重时整体压暗 35%，
    // 比单纯混色更像白内障（混浊既偏黄也变暗）。
    result.convertTo(result, -1, 1.0 - 0.35 * amount, 0.0);
    return result;
}

}
