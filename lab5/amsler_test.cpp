#include "amsler_test.h"

#include <algorithm>
#include <numeric>
#include <utility>

namespace lab5 {

// 默认构造：用默认网格配置和 std::random_device 提供的种子，委托给下面的主构造函数。
AmslerTest::AmslerTest()
    : AmslerTest(GridConfig{}, std::random_device{}())
{
}

// 主构造函数：先把网格与采样参数钳到合法范围，再按 row * cols + col 给每个区域写入网格坐标。
// 只在构造时执行一次，之后网格坐标不再变化。
AmslerTest::AmslerTest(GridConfig config, std::uint64_t seed)
    : m_config(std::move(config)), m_rng(seed)
{
    // 行列都至少为 1，且总数不超过 kMaxRois；先钳好 rows，cols 才能用 kMaxRois / rows 作为上界。
    m_config.rows = std::max(1, std::min(m_config.rows, kMaxRois));
    m_config.cols = std::max(1, std::min(m_config.cols, kMaxRois / m_config.rows));
    // 至少 1 次；若为 0 或负数，区域只要答一次“正常”就会被判定通过，且只显示过第一条刺激。
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

// 开始一轮测试：重排区域访问顺序、清掉上一轮的变形标记与全部游标，然后进入显示刺激阶段。
// 由 AmslerWidget::start() 在 GUI 线程调用；区域数为 0 时直接置 Finished（正常配置下不会发生）。
void AmslerTest::start()
{
    const int roiCount = m_config.rows * m_config.cols;
    m_order.resize(static_cast<std::size_t>(roiCount));
    // 生成 0..roiCount-1 再洗牌：访问顺序随机，受试者无法按固定方位预期下一格。
    std::iota(m_order.begin(), m_order.end(), 0);
    std::shuffle(m_order.begin(), m_order.end(), m_rng);
    // 逐区域清零，本轮统计从头开始。
    for (int index = 0; index < roiCount; ++index)
        m_rois[static_cast<std::size_t>(index)].distorted = false;
    // 游标、线型计数、已采样数必须一起归零，否则三者会指向不同的区域。
    m_cursor = 0;
    m_lineIndex = 0;
    m_sampled = 0;
    m_phase = m_order.empty() ? Phase::Finished : Phase::ShowingStimulus;
}

// 提交一次作答，只在 WaitingAnswer 阶段被接受：其余阶段（刺激还在显示、已结束、未开始）一律忽略。
// 判定规则：答“弯曲”1 次就把当前区域标为变形并前进到下一区域；
// 答“正常”要连续累计够 samplesPerRoi（默认 3）次才前进，否则回到显示刺激阶段换下一种线型。
// 由 AmslerWidget::answerNormal() / answerDistorted() 在 GUI 线程调用。
void AmslerTest::answer(Answer answer)
{
    // 阶段不符时返回：界面在刺激显示期间也会转发按键，这里必须挡住，否则会提前推进。
    if (m_phase != Phase::WaitingAnswer || m_order.empty())
        return;

    if (answer == Answer::Distorted) {
        // 一次“弯曲”即可定论，不再要求答满 3 次。
        currentRoi().distorted = true;
        advanceRoi();
        return;
    }

    // 答“正常”只累加计数；m_lineIndex 同时决定该区域下一条刺激的线型（见 patternFor）。
    ++m_lineIndex;
    if (m_lineIndex >= m_config.samplesPerRoi)
        advanceRoi();
    else
        m_phase = Phase::ShowingStimulus;
}

// 刺激显示时间到：ShowingStimulus → WaitingAnswer，此后 answer() 才会受理作答。
// 计时不由本类负责，而是 AmslerWidget 的单次定时器到点后调用这里，所以本类不保存时间状态。
void AmslerTest::stimulusTimeout()
{
    // 只有正在显示刺激时才有意义；其他阶段（例如已结束）的迟到回调直接忽略。
    if (m_phase == Phase::ShowingStimulus)
        m_phase = Phase::WaitingAnswer;
}

// 判断测试是否仍在进行：显示刺激与等待作答都算运行中，Idle/Finished 不算。
// 界面在每次按键前调用它，用来滤掉未开始或已结束时的点击。
bool AmslerTest::running() const
{
    return m_phase == Phase::ShowingStimulus || m_phase == Phase::WaitingAnswer;
}

// 取当前应显示的刺激（当前区域 + 当前线型），只在 ShowingStimulus 阶段有值。
// 由 AmslerWidget::paintEvent() 在 GUI 线程调用；WaitingAnswer 阶段返回空，
// 界面因此不会在受试者作答期间继续显示图形。
std::optional<Stimulus> AmslerTest::currentStimulus() const
{
    if (m_phase != Phase::ShowingStimulus || m_order.empty())
        return std::nullopt;
    Stimulus stimulus;
    // 经过 m_order 做一层间接：m_cursor 是访问次序，m_order[m_cursor] 才是区域下标。
    stimulus.roi = m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])];
    // 线型由当前区域已答“正常”的次数决定。
    stimulus.pattern = patternFor(m_lineIndex);
    return stimulus;
}

// 按网格坐标返回区域，线性下标是行优先的 row * cols + col。
// 由 AmslerWidget::drawGrid() 在画完整网格时逐格调用；不做越界检查，调用者保证坐标合法。
const Roi &AmslerTest::roiAt(int row, int col) const
{
    return m_rois[static_cast<std::size_t>(row * m_config.cols + col)];
}

// 汇总结果：sampledRois 取已走过的区域数，distortedRois 统计被标记的区域，每个区域最多计一次。
// 由 AmslerWidget 在测试结束时调用（也可随时调用，返回的是当前快照）。
TestResult AmslerTest::result() const
{
    TestResult result;
    result.sampledRois = m_sampled;
    for (int index = 0; index < m_config.rows * m_config.cols; ++index) {
        if (m_rois[static_cast<std::size_t>(index)].distorted) {
            ++result.distortedRois;
            // 编号从 1 开始，与 drawGrid() 在每格左上角标注的编号一致。
            result.distortedIndices.push_back(index + 1);
        }
    }
    return result;
}

// 当前区域的写引用，供 answer() 打变形标记；调用前 m_order 必须非空。
Roi &AmslerTest::currentRoi()
{
    return m_rois[static_cast<std::size_t>(m_order[static_cast<std::size_t>(m_cursor)])];
}

// 线型与线型序号一一对应：0 → \，1 → /，2 → X。
// 因此同一区域依次显示这三条刺激，连答 3 次“正常”正好走完一轮。
LinePattern AmslerTest::patternFor(int lineIndex) const
{
    // 取模：lineIndex 超过 2 时（例如 samplesPerRoi 被调大）仍然回到合法线型。
    switch (lineIndex % 3) {
    case 0:
        return LinePattern::Backslash;
    case 1:
        return LinePattern::Slash;
    default:
        return LinePattern::Cross;
    }
}

// 结束当前区域：已采样数 +1、线型计数清零，并把游标移到下一区域。
// 还有下一区域则进入显示刺激阶段准备新的刺激；已走完全部区域则置 Finished 并返回 false。
// 返回值目前没有调用者使用（answer() 不检查它），阶段切换完全靠这里对 m_phase 的赋值。
bool AmslerTest::advanceRoi()
{
    // 每个区域只在这里计一次，所以无论是答满 3 次“正常”还是 1 次“弯曲”走到这一步，分母都只 +1。
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
