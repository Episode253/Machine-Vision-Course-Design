#ifndef LAB8_AMSLER_TEST_H
#define LAB8_AMSLER_TEST_H

#include <array>
#include <cstdint>
#include <optional>
#include <random>
#include <vector>

namespace lab8 {
// 刺激的三种线型，依 lineIndex 顺序循环出现。
enum class LinePattern { Backslash, Slash, Cross };
// 受试者的两种作答。
enum class Answer { Normal, Distorted };
// 测试阶段。比 lab5 那份多一个 WaitingFixation：注视无效时停在这里，既不显示刺激也不接受作答。
enum class Phase { Idle, ShowingStimulus, WaitingAnswer, Finished, WaitingFixation };
// 一个网格区域：行列号 + 是否已被判为变形。
struct Roi { int row = 0; int col = 0; bool distorted = false; };
// 网格配置，只有这三项（lab5 那份还带逻辑宽高）。
struct GridConfig { int rows = 8; int cols = 8; int samplesPerRoi = 3; };
// 一次刺激：区域 + 该次要画的线型。
struct Stimulus { Roi roi; LinePattern pattern = LinePattern::Backslash; };
// 测试结果：已完成作答的区域数、被判为变形的区域数。
struct TestResult { int sampledRois = 0; int distortedRois = 0; };

class AmslerTest {
public:
    // 区域总数上限，同时是 m_rois 的容量；默认网格 8×8 正好用满。
    static constexpr int kMaxRois = 64;
    AmslerTest();
    explicit AmslerTest(GridConfig config, std::uint64_t seed);
    void start();
    // 作废当前试次：只重置 m_lineIndex 与阶段，见 .cpp。
    void invalidateCurrentTrial();
    void answer(Answer answer);
    // 闪烁计时结束时由 AmslerWidget 调用。
    void stimulusTimeout();
    // 注视状态变化的唯一入口，两种转移见 .cpp。
    void setFixationValid(bool valid);
    bool running() const;
    bool finished() const { return m_phase == Phase::Finished; }
    bool fixationValid() const { return m_fixationValid; }
    Phase phase() const { return m_phase; }
    std::optional<Stimulus> currentStimulus() const;
    const GridConfig &config() const { return m_config; }
    const Roi &roiAt(int row, int col) const;
    TestResult result() const;
private:
    Roi &currentRoi();
    LinePattern patternFor(int lineIndex) const;
    bool advanceRoi();
    GridConfig m_config;
    // 定长数组，容量 kMaxRois，只有前 rows*cols 项被使用。
    std::array<Roi, kMaxRois> m_rois;
    // 区域下标的随机访问顺序；取当前区域时要先用 m_cursor 从它取下标。
    std::vector<int> m_order;
    std::mt19937_64 m_rng;
    // m_order 中的当前位置。
    int m_cursor = 0;
    // 当前区域内已连续答「正常」的次数。
    int m_lineIndex = 0;
    // 已完成作答的区域数，直接计入结果。
    int m_sampled = 0;
    Phase m_phase = Phase::Idle;
    // 最近一次注视判定，用于门控显示刺激与接受作答。
    bool m_fixationValid = false;
};
}
#endif
