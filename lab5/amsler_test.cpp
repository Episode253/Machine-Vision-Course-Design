#include "amsler_test.h"

#include <algorithm>
#include <numeric>
#include <utility>

namespace lab5 {

AmslerTest::AmslerTest()
    : AmslerTest(GridConfig{}, std::random_device{}())
{
}

AmslerTest::AmslerTest(GridConfig config, std::uint64_t seed)
    : m_config(std::move(config)), m_rng(seed)
{
    m_config.rows = std::max(1, std::min(m_config.rows, kMaxRois));
    m_config.cols = std::max(1, std::min(m_config.cols, kMaxRois / m_config.rows));
    m_config.samplesPerRoi = std::max(1, m_config.samplesPerRoi);
    for (int row = 0; row < m_config.rows; ++row) {
        for (int col = 0; col < m_config.cols; ++col) {
            Roi &roi = m_rois[static_cast<std::size_t>(row * m_config.cols + col)];
            roi.row = row;
            roi.col = col;
            roi.distorted = false;
        }
    }
}

void AmslerTest::start()
{
    // TODO(lab5): 生成区域访问顺序并随机打乱。
    // TODO(lab5): 复位各区域的变形标记与游标计数。
    // TODO(lab5): 进入“显示刺激”阶段。
    m_phase = Phase::Idle;
}

void AmslerTest::answer(Answer answer)
{
    // TODO(lab5): 未在测试中就忽略本次回答。
    // TODO(lab5): 答“弯曲”直接标记当前区域并前进到下一区域。
    // TODO(lab5): 答“正常”需累计够线型数才前进，否则继续下一种线型。
    (void)answer;
}

void AmslerTest::stimulusTimeout()
{
    if (m_phase == Phase::ShowingStimulus)
        m_phase = Phase::WaitingAnswer;
}

bool AmslerTest::running() const
{
    return m_phase == Phase::ShowingStimulus || m_phase == Phase::WaitingAnswer;
}

std::optional<Stimulus> AmslerTest::currentStimulus() const
{
    if (m_phase != Phase::ShowingStimulus || m_order.empty())
        return std::nullopt;
    Stimulus stimulus;
    stimulus.roi = m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])];
    stimulus.pattern = patternFor(m_lineIndex);
    return stimulus;
}

const Roi &AmslerTest::roiAt(int row, int col) const
{
    return m_rois[static_cast<std::size_t>(row * m_config.cols + col)];
}

TestResult AmslerTest::result() const
{
    TestResult result;
    result.sampledRois = m_sampled;
    for (int index = 0; index < m_config.rows * m_config.cols; ++index) {
        if (m_rois[static_cast<std::size_t>(index)].distorted) {
            ++result.distortedRois;
            result.distortedIndices.push_back(index + 1);
        }
    }
    return result;
}

Roi &AmslerTest::currentRoi()
{
    return m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])];
}

LinePattern AmslerTest::patternFor(int lineIndex) const
{
    switch (lineIndex % 3) {
    case 0:
        return LinePattern::Backslash;
    case 1:
        return LinePattern::Slash;
    default:
        return LinePattern::Cross;
    }
}

bool AmslerTest::advanceRoi()
{
    ++m_sampled;
    m_lineIndex = 0;
    if (++m_cursor >= static_cast<int>(m_order.size())) {
        m_phase = Phase::Finished;
        return false;
    }
    m_phase = Phase::ShowingStimulus;
    return true;
}

}
