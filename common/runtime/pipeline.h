#ifndef VISUAL_LAB_RUNTIME_PIPELINE_H
#define VISUAL_LAB_RUNTIME_PIPELINE_H

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "core/bounded_queue.hpp"
#include "core/stage.hpp"
#include "runtime/worker_thread.h"

namespace labruntime {





class Pipeline {
public:
    Pipeline();
    ~Pipeline();
    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;
    Pipeline(Pipeline &&) = delete;
    Pipeline &operator=(Pipeline &&) = delete;


    void setSource(std::unique_ptr<labcore::Source> source);




    void setSource(labcore::Source *source);
    void addStage(std::unique_ptr<labcore::Stage> stage);
    void setOutput(labcore::OutputFn output);
    void setQueueCapacity(std::size_t capacity);
    void setQueuePolicy(labcore::OverflowPolicy policy);



    [[nodiscard]] bool build();

    [[nodiscard]] bool start();
    void stop();
    bool running() const { return m_running.load(); }
    std::string lastError() const;

    struct StageStat {
        std::string name;
        std::uint64_t in = 0;
        std::uint64_t out = 0;
        std::uint64_t dropped = 0;
        double avgMs = 0.0;
    };

    struct Stats {
        bool running = false;
        double fps = 0.0;
        std::uint64_t produced = 0;
        std::uint64_t consumed = 0;
        std::uint64_t dropped = 0;
        std::vector<StageStat> stages;
    };

    Stats stats() const;

private:
    struct StageCounter {
        std::atomic<std::uint64_t> in{0};
        std::atomic<std::uint64_t> out{0};
        std::atomic<std::uint64_t> dropped{0};
        std::atomic<std::uint64_t> totalUs{0};
    };

    void setError(std::string message);
    labcore::FramePtr runInlineChain(std::size_t startIndex, labcore::FramePtr frame,
                                     std::size_t *nextStageIndex);
    void pushToNext(std::size_t stageIndex, labcore::FramePtr frame);
    void runSource();
    void runStage(std::size_t stageIndex);
    void closeQueues();
    void joinWorkers();

    std::unique_ptr<labcore::Source> m_ownedSource;
    labcore::Source *m_source = nullptr;
    bool m_ownsSource = false;
    std::vector<std::unique_ptr<labcore::Stage>> m_stages;
    labcore::OutputFn m_output;
    std::size_t m_queueCapacity = 2;
    labcore::OverflowPolicy m_queuePolicy = labcore::OverflowPolicy::DropOldest;

    std::vector<std::unique_ptr<labcore::BoundedQueue<labcore::FramePtr>>> m_stageQueues;
    std::vector<std::unique_ptr<labruntime::WorkerThread>> m_workers;
    std::vector<std::unique_ptr<StageCounter>> m_counters;

    std::atomic_bool m_running{false};
    std::atomic<std::uint64_t> m_produced{0};
    std::atomic<std::uint64_t> m_consumed{0};
    std::chrono::steady_clock::time_point m_startTime{};

    mutable std::mutex m_errorMutex;
    std::string m_error;
};

}

#endif
