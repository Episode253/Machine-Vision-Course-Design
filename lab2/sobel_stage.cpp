#include "sobel_stage.h"

#include <memory>
#include <utility>

#include <opencv2/imgproc.hpp>

// 构造：先建好默认参数对象，使 process 在未收到 setParams 时也能直接运行。
// 默认只求水平方向的一阶差分（dx=1、dy=0、ksize=3）。
SobelStage::SobelStage()
    : m_params(std::make_shared<const SobelParams>())
{
}

// 输入 Gray8、输出 Gray8；本类没有覆写 execution，用的是基类默认的 OwnThread，
// 因此 process 跑在管线为它单独开的工作线程上，与 setParams 所在线程不同。
labcore::FramePtr SobelStage::process(const labcore::FramePtr &input)
{
    // 上游为空或帧为空时返回空，管线按丢帧统计。
    if (!input || input->mat.empty())
        return nullptr;
    // 只在锁内拷出 shared_ptr，算法本体不持锁：参数对象是 const 的，
    // 一次快照即可覆盖整帧，不会读到半新半旧的参数。
    std::shared_ptr<const SobelParams> params;
    {
        std::lock_guard<std::mutex> lock(m_paramsMutex);
        params = m_params;
    }
    // 中间结果必须用 CV_16S：dx/dy 的梯度有正有负且可能超出 8 位范围，
    // 用 CV_8U 会截断；convertScaleAbs 再取绝对值并饱和回 CV_8U，才能显示。
    cv::Mat edges;
    cv::Sobel(input->mat, edges, CV_16S, params->dx, params->dy, params->ksize);
    cv::convertScaleAbs(edges, edges);
    // 同样保留输入的 seq，格式由单通道推断为 Gray8。
    return std::make_shared<const labcore::Frame>(labcore::makeFrame(std::move(edges), input->seq));
}

// 参数热更新入口，按设计由 GUI 线程调用（lab2 的界面目前没有接调参控件）。
// dynamic_pointer_cast 失败说明参数类型不符，此时静默保留旧参数，不把阶段置成半配置状态。
void SobelStage::setParams(std::shared_ptr<const labcore::StageParams> params)
{
    if (auto typed = std::dynamic_pointer_cast<const SobelParams>(std::move(params))) {
        std::lock_guard<std::mutex> lock(m_paramsMutex);
        m_params = std::move(typed);
    }
}
