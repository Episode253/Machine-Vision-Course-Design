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
    const int roiCount = m_config.rows * m_config.cols;
    m_order.resize(static_cast<std::size_t>(roiCount));
    std::iota(m_order.begin(), m_order.end(), 0);
    std::shuffle(m_order.begin(), m_order.end(), m_rng);
    for (int index = 0; index < roiCount; ++index)
        m_rois[static_cast<std::size_t>(index)].distorted = false;
    m_cursor = 0;
    m_lineIndex = 0;
    m_sampled = 0;
    m_phase = m_order.empty() ? Phase::Finished : Phase::ShowingStimulus;
}

void AmslerTest::answer(Answer answer)
{
    if (m_phase != Phase::WaitingAnswer || m_order.empty())
        return;

    if (answer == Answer::Distorted) {
        currentRoi().distorted = true;
        advanceRoi();
        return;
    }

    ++m_lineIndex;
    if (m_lineIndex >= m_config.samplesPerRoi)
        advanceRoi();
    else
        m_phase = Phase::ShowingStimulus;
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
