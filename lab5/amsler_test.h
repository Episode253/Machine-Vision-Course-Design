#ifndef LAB5_AMSLER_TEST_H
#define LAB5_AMSLER_TEST_H

#include <array>
#include <cstdint>
#include <optional>
#include <random>
#include <vector>

namespace lab5 {

enum class LinePattern { Backslash, Slash, Cross };
enum class Answer { Normal, Distorted };
enum class Phase { Idle, ShowingStimulus, WaitingAnswer, Finished };

struct Roi {
    int row = 0;
    int col = 0;
    bool distorted = false;
};

struct GridConfig {
    int rows = 8;
    int cols = 8;
    int logicalWidth = 1000;
    int logicalHeight = 1000;
    int samplesPerRoi = 3;
};

struct Stimulus {
    Roi roi;
    LinePattern pattern = LinePattern::Backslash;
};

struct TestResult {
    int sampledRois = 0;
    int distortedRois = 0;
    std::vector<int> distortedIndices;
};





class AmslerTest {
public:
    static constexpr int kMaxRois = 64;

    AmslerTest();
    explicit AmslerTest(GridConfig config, std::uint64_t seed);



    void start();





    void answer(Answer answer);


    void stimulusTimeout();

    Phase phase() const { return m_phase; }
    bool running() const;
    bool finished() const { return m_phase == Phase::Finished; }
    std::optional<Stimulus> currentStimulus() const;

    const GridConfig &config() const { return m_config; }
    const Roi &roiAt(int row, int col) const;
    TestResult result() const;

private:
    Roi &currentRoi();
    LinePattern patternFor(int lineIndex) const;

    bool advanceRoi();

    GridConfig m_config;
    std::array<Roi, kMaxRois> m_rois;
    std::vector<int> m_order;
    std::mt19937_64 m_rng;
    int m_cursor = 0;
    int m_lineIndex = 0;
    int m_sampled = 0;
    Phase m_phase = Phase::Idle;
};

}

#endif
