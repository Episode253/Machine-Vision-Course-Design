#include "amsler_test.h"
#include <algorithm>
#include <numeric>

namespace lab8 {
AmslerTest::AmslerTest() : AmslerTest(GridConfig{}, std::random_device{}()) {}
AmslerTest::AmslerTest(GridConfig config, std::uint64_t seed) : m_config(config), m_rng(seed)
{
    m_config.rows = std::max(1, std::min(m_config.rows, kMaxRois));
    m_config.cols = std::max(1, std::min(m_config.cols, kMaxRois / m_config.rows));
    m_config.samplesPerRoi = std::max(1, m_config.samplesPerRoi);
    for (int row = 0; row < m_config.rows; ++row)
        for (int col = 0; col < m_config.cols; ++col)
            m_rois[static_cast<std::size_t>(row * m_config.cols + col)] = {row, col, false};
}
void AmslerTest::start()
{
    const int count = m_config.rows * m_config.cols;
    m_order.resize(static_cast<std::size_t>(count));
    std::iota(m_order.begin(), m_order.end(), 0);
    std::shuffle(m_order.begin(), m_order.end(), m_rng);
    for (int i = 0; i < count; ++i) m_rois[static_cast<std::size_t>(i)].distorted = false;
    m_cursor = m_lineIndex = m_sampled = 0;
    m_phase = count == 0 ? Phase::Finished : (m_fixationValid ? Phase::ShowingStimulus : Phase::WaitingFixation);
}
void AmslerTest::setFixationValid(bool valid)
{
    const bool changed = valid != m_fixationValid;
    m_fixationValid = valid;
    if (!changed || m_phase == Phase::Idle || m_phase == Phase::Finished) return;
    if (!valid) { if (running()) invalidateCurrentTrial(); m_phase = Phase::WaitingFixation; }
    else if (m_phase == Phase::WaitingFixation && !m_order.empty()) m_phase = Phase::ShowingStimulus;
}
void AmslerTest::invalidateCurrentTrial()
{
    if (m_order.empty() || m_phase == Phase::Idle || m_phase == Phase::Finished) return;
    m_lineIndex = 0;
    m_phase = Phase::WaitingFixation;
}
void AmslerTest::answer(Answer answer)
{
    if (m_phase != Phase::WaitingAnswer || !m_fixationValid || m_order.empty()) return;
    if (answer == Answer::Distorted) { currentRoi().distorted = true; advanceRoi(); return; }
    if (++m_lineIndex >= m_config.samplesPerRoi) advanceRoi();
    else m_phase = Phase::ShowingStimulus;
}
void AmslerTest::stimulusTimeout() { if (m_phase == Phase::ShowingStimulus && m_fixationValid) m_phase = Phase::WaitingAnswer; }
bool AmslerTest::running() const { return m_phase == Phase::ShowingStimulus || m_phase == Phase::WaitingAnswer; }
std::optional<Stimulus> AmslerTest::currentStimulus() const
{
    if (m_phase != Phase::ShowingStimulus || m_order.empty()) return std::nullopt;
    return Stimulus{m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])], patternFor(m_lineIndex)};
}
const Roi &AmslerTest::roiAt(int row, int col) const { return m_rois[static_cast<std::size_t>(row * m_config.cols + col)]; }
TestResult AmslerTest::result() const
{
    TestResult result; result.sampledRois = m_sampled;
    for (int i = 0; i < m_config.rows * m_config.cols; ++i)
        if (m_rois[static_cast<std::size_t>(i)].distorted) ++result.distortedRois;
    return result;
}
Roi &AmslerTest::currentRoi() { return m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])]; }
LinePattern AmslerTest::patternFor(int index) const { return index % 3 == 0 ? LinePattern::Backslash : index % 3 == 1 ? LinePattern::Slash : LinePattern::Cross; }
bool AmslerTest::advanceRoi()
{
    ++m_sampled; m_lineIndex = 0;
    if (++m_cursor >= static_cast<int>(m_order.size())) { m_phase = Phase::Finished; return false; }
    m_phase = m_fixationValid ? Phase::ShowingStimulus : Phase::WaitingFixation; return true;
}
}
