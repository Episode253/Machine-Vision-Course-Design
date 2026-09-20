#include "calibration_session.h"

#include <cstdlib>

namespace lab6 {

void CalibrationSession::reset()
{
    m_centre = cv::Point();
    m_samples = 0;
}

void CalibrationSession::observe(const EyeDetectionResult &detection)
{
    if (!detection.found)
        return;

    m_centre.x = static_cast<int>((static_cast<long long>(m_centre.x) * m_samples
                                   + detection.center.x) / (m_samples + 1));
    m_centre.y = static_cast<int>((static_cast<long long>(m_centre.y) * m_samples
                                   + detection.center.y) / (m_samples + 1));
    ++m_samples;
}

CalibrationOutcome CalibrationSession::evaluate(const EyeDetectionResult &actual,
                                                int tolerance) const
{
    return evaluateAgainst(actual, m_centre, tolerance);
}

CalibrationOutcome CalibrationSession::evaluateAgainst(const EyeDetectionResult &actual,
                                                       const cv::Point &centre, int tolerance) const
{
    CalibrationOutcome outcome;
    outcome.centre = centre;
    if (m_samples == 0) {
        outcome.status = CalibrationOutcome::Status::Idle;
        return outcome;
    }
    if (!actual.found) {
        outcome.status = CalibrationOutcome::Status::NoEye;
        return outcome;
    }
    outcome.actual = actual.center;
    outcome.offsetX = actual.center.x - centre.x;
    if (std::abs(outcome.offsetX) < tolerance)
        outcome.status = CalibrationOutcome::Status::Valid;
    else
        outcome.status = outcome.offsetX < 0 ? CalibrationOutcome::Status::Left
                                             : CalibrationOutcome::Status::Right;
    return outcome;
}

}
