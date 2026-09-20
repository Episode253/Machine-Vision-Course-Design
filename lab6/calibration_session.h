#ifndef LAB6_CALIBRATION_SESSION_H
#define LAB6_CALIBRATION_SESSION_H

#include <cstdint>

#include <opencv2/core.hpp>

#include "eye_detection.h"

namespace lab6 {



struct CalibrationOutcome {
    enum class Status { Idle, Valid, Left, Right, Divergent, NoEye, Timeout };

    Status status = Status::Idle;
    cv::Point centre{};
    cv::Point actual{};
    int offsetX = 0;

    bool valid() const { return status == Status::Valid; }
};



class CalibrationSession {
public:
    void reset();

    void observe(const EyeDetectionResult &detection);
    bool calibrated() const { return m_samples > 0; }
    cv::Point centre() const { return m_centre; }
    std::uint32_t samples() const { return m_samples; }

    CalibrationOutcome evaluate(const EyeDetectionResult &actual, int tolerance = 4) const;



    CalibrationOutcome evaluateAgainst(const EyeDetectionResult &actual, const cv::Point &centre,
                                       int tolerance = 4) const;

private:
    cv::Point m_centre{};
    std::uint32_t m_samples = 0;
};

}

#endif
