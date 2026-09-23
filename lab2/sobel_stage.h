#ifndef LAB2_SOBEL_STAGE_H
#define LAB2_SOBEL_STAGE_H

#include <memory>
#include <mutex>
#include <string_view>

#include "core/stage.hpp"



// Sobel 算法参数：dx/dy 为求导阶数，ksize 为核尺寸（须是 1、3、5、7 这类奇数）。
// 继承 StageParams 只是为了让参数能经基类指针通过 setParams 传递。
struct SobelParams : labcore::StageParams {
    int dx = 1;
    int dy = 0;
    int ksize = 3;
};

class SobelStage : public labcore::Stage {
public:
    SobelStage();

    std::string_view name() const override { return "sobel"; }
    // 输入格式声明为 Gray8：管线 build 时按它校验格式链，
    // 所以灰度阶段必须排在本阶段之前，顺序反了会在 build 阶段失败。
    labcore::PixelFormat inputFormat() const override { return labcore::PixelFormat::Gray8; }
    labcore::PixelFormat outputFormat() const override { return labcore::PixelFormat::Gray8; }



    // 本类没有覆写 execution，用基类默认的 OwnThread：管线会为它开一个工作线程
    // 并插一级有界队列，process 在该线程执行，因此参数需要加锁保护。
    // 于是本阶段与前面的灰度阶段执行方式不同：gray 为 Inline、sobel 为 OwnThread。
    labcore::FramePtr process(const labcore::FramePtr &input) override;



    // 参数热更新入口：按设计由 GUI 线程调用，与 process 所在线程不同，所以必须加锁。
    // lab2 的界面目前没有调用它，参数保持默认值。
    void setParams(std::shared_ptr<const labcore::StageParams> params) override;

private:
    // 当前参数对象；只整体替换、不原地修改，读方拿到的快照始终自洽。
    std::shared_ptr<const SobelParams> m_params;
    mutable std::mutex m_paramsMutex;
};

#endif
