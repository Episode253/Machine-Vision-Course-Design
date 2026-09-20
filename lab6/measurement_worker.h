#ifndef LAB6_MEASUREMENT_WORKER_H
#define LAB6_MEASUREMENT_WORKER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "core/frame.hpp"
#include "eye_detection.h"
#include "runtime/frame_source.h"
#include "runtime/worker_thread.h"





class MeasurementWorker {
public:
    using PreviewCallback = std::function<void(labcore::FramePtr, const EyeDetectionResult &)>;

    MeasurementWorker() = default;
    ~MeasurementWorker();

    MeasurementWorker(const MeasurementWorker &) = delete;
    MeasurementWorker &operator=(const MeasurementWorker &) = delete;

    bool open(std::string *error);
    void close();
    bool isOpen() const { return m_open.load(); }



    bool startPreview(PreviewCallback onFrame);
    void stopPreview();
    bool previewRunning() const { return m_previewRunning.load(); }


    void setParams(const DetectParams &params);
    DetectParams params() const;






    std::optional<EyeDetectionResult> measureOnce(std::chrono::milliseconds timeout);

private:
    void runPreview();
    EyeDetectionResult detect(const labcore::FramePtr &frame) const;

    FrameSource m_source;
    labruntime::WorkerThread m_previewWorker;
    PreviewCallback m_onFrame;
    std::atomic_bool m_stopRequested{false};
    std::atomic_bool m_previewRunning{false};
    std::atomic_bool m_open{false};

    mutable std::mutex m_paramsMutex;
    DetectParams m_params;

    mutable std::mutex m_errorMutex;
    std::string m_error;
};

#endif
