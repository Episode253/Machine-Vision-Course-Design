#include "amsler_test.h"
#include <algorithm>
#include <numeric>

namespace lab8 {
// 默认构造：用默认 GridConfig 和一个随机种子转交下面的构造函数。
AmslerTest::AmslerTest() : AmslerTest(GridConfig{}, std::random_device{}()) {}
// 主构造：先把 rows/cols/samplesPerRoi 夹到合法范围（m_rois 是定长 kMaxRois 数组，越界会写坏内存），
// 再按行列给每个区域填上自己的位置。m_rng 的种子决定 start() 里访问顺序的打乱结果。
AmslerTest::AmslerTest(GridConfig config, std::uint64_t seed) : m_config(config), m_rng(seed)
{
    m_config.rows = std::max(1, std::min(m_config.rows, kMaxRois));
    m_config.cols = std::max(1, std::min(m_config.cols, kMaxRois / m_config.rows));
    m_config.samplesPerRoi = std::max(1, m_config.samplesPerRoi);
    for (int row = 0; row < m_config.rows; ++row)
        for (int col = 0; col < m_config.cols; ++col)
            m_rois[static_cast<std::size_t>(row * m_config.cols + col)] = {row, col, false};
}
// 开始一次测试，调用者：AmslerWidget::start()（GUI 线程）。
// 生成区域访问顺序并随机打乱，避免受试者记住方位；清掉上一轮的变形标记并复位三个计数；
// 起始阶段取决于当前注视判定：有效就直接显示刺激，无效先停在等待注视。
void AmslerTest::start()
{
    const int count = m_config.rows * m_config.cols;
    m_order.resize(static_cast<std::size_t>(count));
    std::iota(m_order.begin(), m_order.end(), 0);
    // 打乱访问顺序：Amsler 检查要靠随机化掩盖受试者对下一格方位的预期。
    std::shuffle(m_order.begin(), m_order.end(), m_rng);
    for (int i = 0; i < count; ++i) m_rois[static_cast<std::size_t>(i)].distorted = false;
    m_cursor = m_lineIndex = m_sampled = 0;
    // 三选一：没有区域直接判完成；注视有效才显示刺激，否则先等注视（WaitingFixation）。
    m_phase = count == 0 ? Phase::Finished : (m_fixationValid ? Phase::ShowingStimulus : Phase::WaitingFixation);
}
// 注视状态变化的唯一入口，由 AmslerWidget::setFixationValid 转发（源头是 FixationMonitor 的
// fixationChanged，在 GUI 线程执行）。两种转移，且只处理"变化"：
//   失去注视 → 若正在测试则作废当前试次（invalidateCurrentTrial，内部也置 WaitingFixation），阶段转 WaitingFixation；
//   恢复注视 → 仅当停留在 WaitingFixation 且还有区域时回到 ShowingStimulus，此时游标未动，即继续同一个区域。
// 同值重复调用以及 Idle/Finished 阶段一律忽略。
void AmslerTest::setFixationValid(bool valid)
{
    const bool changed = valid != m_fixationValid;
    m_fixationValid = valid;
    if (!changed || m_phase == Phase::Idle || m_phase == Phase::Finished) return;
    // 恢复分支要求 m_order 非空，否则回到 ShowingStimulus 后也取不出刺激。
    if (!valid) { if (running()) invalidateCurrentTrial(); m_phase = Phase::WaitingFixation; }
    else if (m_phase == Phase::WaitingFixation && !m_order.empty()) m_phase = Phase::ShowingStimulus;
}
// 作废当前试次：只把 m_lineIndex 归零（当前区域那 3 次线型重头再来）并把阶段置为 WaitingFixation。
// 刻意不动 m_cursor（不跳过区域）、m_sampled（不计作答）、任何 distorted 标记（作答不算数）。
// 守卫：没有区域或处于 Idle/Finished 时直接返回。
void AmslerTest::invalidateCurrentTrial()
{
    if (m_order.empty() || m_phase == Phase::Idle || m_phase == Phase::Finished) return;
    m_lineIndex = 0;
    m_phase = Phase::WaitingFixation;
}
// 作答，由 AmslerWidget::answerNormal/answerDistorted 调用（GUI 线程）。
// 守卫三件套：必须处于 WaitingAnswer（等闪烁计时结束）、注视有效、且还有待答区域，否则忽略。
// 答「弯曲」：立刻标记当前区域变形并前进（1 次即判定，不累计）。
// 答「正常」：累加 m_lineIndex，够 samplesPerRoi 次才前进，否则回到 ShowingStimulus 再闪一次当前线型。
void AmslerTest::answer(Answer answer)
{
    if (m_phase != Phase::WaitingAnswer || !m_fixationValid || m_order.empty()) return;
    if (answer == Answer::Distorted) { currentRoi().distorted = true; advanceRoi(); return; }
    if (++m_lineIndex >= m_config.samplesPerRoi) advanceRoi();
    else m_phase = Phase::ShowingStimulus;
}
// 单个刺激的闪烁计时（AmslerWidget 的 250ms 定时器）到期时调用：把阶段推进到 WaitingAnswer，
// 此后才接受作答。注视无效时不推进，避免无人注视时进入可作答状态。
void AmslerTest::stimulusTimeout() { if (m_phase == Phase::ShowingStimulus && m_fixationValid) m_phase = Phase::WaitingAnswer; }
bool AmslerTest::running() const { return m_phase == Phase::ShowingStimulus || m_phase == Phase::WaitingAnswer; }
// 返回当前要显示的刺激（区域 + 线型）：只有 ShowingStimulus 阶段且还有区域时有效，
// 其余阶段返回空，AmslerWidget::paintEvent 因此不会画出任何格子。
std::optional<Stimulus> AmslerTest::currentStimulus() const
{
    if (m_phase != Phase::ShowingStimulus || m_order.empty()) return std::nullopt;
    return Stimulus{m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])], patternFor(m_lineIndex)};
}
const Roi &AmslerTest::roiAt(int row, int col) const { return m_rois[static_cast<std::size_t>(row * m_config.cols + col)]; }
// 汇总结果：sampledRois = 已完成作答的区域数，distortedRois = 被标记的区域数。
// distorted 是 bool，所以同一区域无论被标记几次都只贡献 1。
TestResult AmslerTest::result() const
{
    TestResult result; result.sampledRois = m_sampled;
    for (int i = 0; i < m_config.rows * m_config.cols; ++i)
        if (m_rois[static_cast<std::size_t>(i)].distorted) ++result.distortedRois;
    return result;
}
// 当前游标所指区域的引用（m_order 里存的是区域下标，不是行列号，要经它换算）。
Roi &AmslerTest::currentRoi() { return m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])]; }
// 线型按 m_lineIndex 取模 3 循环：0 → 反斜线，1 → 斜线，2 → 交叉，
// 所以同一区域连续 3 次会依次看到三种线型。
LinePattern AmslerTest::patternFor(int index) const { return index % 3 == 0 ? LinePattern::Backslash : index % 3 == 1 ? LinePattern::Slash : LinePattern::Cross; }
// 前进到下一个区域：先 ++m_sampled（该区域算作答完成）、m_lineIndex 归零；游标越界则置 Finished 并返回 false，
// 否则按注视是否有效决定显示刺激还是停在 WaitingFixation。返回是否还有下一个区域。
bool AmslerTest::advanceRoi()
{
    ++m_sampled; m_lineIndex = 0;
    if (++m_cursor >= static_cast<int>(m_order.size())) { m_phase = Phase::Finished; return false; }
    m_phase = m_fixationValid ? Phase::ShowingStimulus : Phase::WaitingFixation; return true;
}
}
