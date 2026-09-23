#ifndef LAB5_AMSLER_TEST_H
#define LAB5_AMSLER_TEST_H

#include <array>
#include <cstdint>
#include <optional>
#include <random>
#include <vector>

namespace lab5 {

// 刺激线型：\ 与 / 是单条对角线，X 由两条对角线叠加而成（绘制见 AmslerWidget::drawStimulus）。
enum class LinePattern { Backslash, Slash, Cross };
// 受试者对当前刺激的回答：只有“正常”和“弯曲”两种。
enum class Answer { Normal, Distorted };
// 测试阶段：Idle 未开始 → ShowingStimulus 显示刺激 → WaitingAnswer 等待作答 → 循环或 Finished 结束。
// 流转规则：start() 进场即 ShowingStimulus；stimulusTimeout() 把它推到 WaitingAnswer，此后才收回答；
// answer() 作答后要么回到 ShowingStimulus（换下一种线型），要么由 advanceRoi() 前进或收尾。
enum class Phase { Idle, ShowingStimulus, WaitingAnswer, Finished };

// 网格中的一格。row/col 是网格坐标；distorted 记录该区域是否被判为变形，每个区域最多计一次。
struct Roi {
    int row = 0;
    int col = 0;
    bool distorted = false;
};

// 网格规模与采样参数：samplesPerRoi = 3，即同一区域要连续答 3 次“正常”才算通过。
// logicalWidth/logicalHeight 是网格的逻辑尺寸，当前代码没有读取它（绘制按控件实际大小等分）。
struct GridConfig {
    int rows = 8;
    int cols = 8;
    int logicalWidth = 1000;
    int logicalHeight = 1000;
    int samplesPerRoi = 3;
};

// 一次刺激 = 当前区域 + 当前要显示／正在显示的线型；由 currentStimulus() 交给界面绘制。
struct Stimulus {
    Roi roi;
    LinePattern pattern = LinePattern::Backslash;
};

// 一轮测试的结果：sampledRois 是已走过的区域数（分母），distortedRois 是其中被判变形的区域数；
// distortedIndices 是变形区域的编号（从 1 开始），与 drawGrid() 在每格左上角标的编号一致。
struct TestResult {
    int sampledRois = 0;
    int distortedRois = 0;
    std::vector<int> distortedIndices;
};

// AmslerTest 是纯逻辑状态机：不继承 QObject、不接触界面、也不创建线程，
// 全部方法都在 GUI 线程被调用，因此内部状态不需要加锁；刺激计时与绘制交给 AmslerWidget。
class AmslerTest {
public:
    // 区域数上限，同时也是 m_rois 数组的容量；构造时把 rows×cols 钳到不超过它。
    static constexpr int kMaxRois = 64;

    // 默认构造：8×8 网格 + std::random_device 提供的随机种子，委托给下面的构造函数。
    AmslerTest();
    // 指定网格与种子；种子固定时区域访问顺序可复现，便于对照同一序列排查问题。
    explicit AmslerTest(GridConfig config, std::uint64_t seed);

    // 开始一轮测试：重排区域访问顺序、清掉上一轮的变形标记与全部游标，进入显示刺激阶段。
    // 由 AmslerWidget::start() 在 GUI 线程调用。
    void start();

    // 提交一次作答：答“弯曲”立即把当前区域标为变形并前进到下一区域；
    // 答“正常”要连续累计够 samplesPerRoi（默认 3）次才前进，否则回到显示刺激阶段换下一种线型。
    // 只在 WaitingAnswer 阶段有效，其余阶段一律忽略；由 AmslerWidget 的两个作答槽在 GUI 线程调用。
    void answer(Answer answer);

    // 刺激显示时间到：把阶段从 ShowingStimulus 推进到 WaitingAnswer，此后才接受作答。
    void stimulusTimeout();

    Phase phase() const { return m_phase; }
    // 是否处于进行中：显示刺激与等待作答都算运行中，Idle/Finished 为 false；界面据此屏蔽无效按键。
    bool running() const;
    bool finished() const { return m_phase == Phase::Finished; }
    // 当前应显示的刺激（区域 + 线型）；非 ShowingStimulus 阶段返回空，界面据此不画刺激。
    std::optional<Stimulus> currentStimulus() const;

    const GridConfig &config() const { return m_config; }
    // 按网格坐标读取区域，供界面在 Idle/Finished 阶段画完整网格。
    const Roi &roiAt(int row, int col) const;
    // 汇总本轮结果（可随时调用，返回当前快照），供界面显示完成文案。
    TestResult result() const;

private:
    // 当前游标所指区域的可写引用；调用者需保证 m_order 非空且 m_cursor 在范围内。
    Roi &currentRoi();
    // 线型按 lineIndex % 3 循环：0 对应 \，1 对应 /，2 对应 X。
    LinePattern patternFor(int lineIndex) const;

    // 前进到下一区域：累计已采样数、清空线型计数；走完全部区域则置 Finished 并返回 false。
    bool advanceRoi();

    // 构造时已把行列与采样数钳到合法范围，之后网格坐标不再变化。
    GridConfig m_config;
    // 全部区域按 row * cols + col 扁平存放；超出 rows×cols 的槽位保持结构体默认值。
    std::array<Roi, kMaxRois> m_rois;
    // 本轮的访问顺序：0..区域数-1 的一个随机排列，m_order[i] 是 m_rois 的下标。
    std::vector<int> m_order;
    // 打乱访问顺序用的随机数引擎；种子固定则整轮顺序可复现。
    std::mt19937_64 m_rng;
    // 在 m_order 中的位置，即当前正在测的区域。
    int m_cursor = 0;
    // 当前区域内已答“正常”的次数，同时决定下一条刺激的线型。
    int m_lineIndex = 0;
    // 已走过的区域数，作为结果报告的分母。
    int m_sampled = 0;
    // 当前阶段，状态机的全部对外行为都由它决定。
    Phase m_phase = Phase::Idle;
};

}

#endif
