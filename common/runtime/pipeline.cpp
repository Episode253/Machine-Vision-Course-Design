#include "pipeline.h"

#include <algorithm>
#include <set>
#include <utility>

namespace labruntime {

Pipeline::Pipeline() = default;

Pipeline::~Pipeline()
{
    stop();
}

void Pipeline::setSource(std::unique_ptr<labcore::Source> source)
{
    if (m_running.load()) {
        setError("cannot change the source of a running pipeline");
        return;
    }
    m_ownedSource = std::move(source);
    m_source = m_ownedSource.get();
    m_ownsSource = true;
}

void Pipeline::setSource(labcore::Source *source)
{
    if (m_running.load()) {
        setError("cannot change the source of a running pipeline");
        return;
    }
    m_ownedSource.reset();
    m_source = source;
    m_ownsSource = false;
}

void Pipeline::addStage(std::unique_ptr<labcore::Stage> stage)
{
    if (m_running.load()) {
        setError("cannot add a stage to a running pipeline");
        return;
    }
    if (stage)
        m_stages.push_back(std::move(stage));
}

void Pipeline::setOutput(labcore::OutputFn output)
{
    m_output = std::move(output);
}

void Pipeline::setQueueCapacity(std::size_t capacity)
{
    m_queueCapacity = capacity == 0 ? 1 : capacity;
}

void Pipeline::setQueuePolicy(labcore::OverflowPolicy policy)
{
    m_queuePolicy = policy;
}

void Pipeline::setError(std::string message)
{
    std::lock_guard<std::mutex> lock(m_errorMutex);
    m_error = std::move(message);
}

std::string Pipeline::lastError() const
{
    std::lock_guard<std::mutex> lock(m_errorMutex);
    return m_error;
}

bool Pipeline::build()
{
    if (!m_source) {
        setError("no frame source configured");
        return false;
    }
    if (!m_output) {
        setError("no output configured");
        return false;
    }

    std::set<std::string> names;
    if (!names.insert(std::string(m_source->name())).second) {
        setError("duplicate node name: " + std::string(m_source->name()));
        return false;
    }

    labcore::PixelFormat expected = m_source->outputFormat();
    for (const auto &stage : m_stages) {
        const std::string name(stage->name());
        if (name.empty()) {
            setError("stage with an empty name");
            return false;
        }
        if (!names.insert(name).second) {
            setError("duplicate node name: " + name);
            return false;
        }
        if (stage->inputFormat() != expected) {
            setError("format mismatch before stage '" + name + "'");
            return false;
        }
        expected = stage->outputFormat();
    }
    return true;
}

bool Pipeline::start()
{
    if (m_running.load())
        return true;
    if (!build())
        return false;

    std::string error;
    if (m_ownsSource && !m_source->open(&error)) {
        setError(error.empty() ? "frame source could not be opened" : error);
        return false;
    }

    m_stageQueues.clear();
    m_stageQueues.resize(m_stages.size());
    m_counters.clear();
    m_counters.reserve(m_stages.size());
    for (std::size_t i = 0; i < m_stages.size(); ++i) {
        if (m_stages[i]->execution() == labcore::Stage::Execution::OwnThread) {
            m_stageQueues[i] = std::make_unique<labcore::BoundedQueue<labcore::FramePtr>>(
                m_queueCapacity, m_queuePolicy);
        }
        m_counters.push_back(std::make_unique<StageCounter>());
    }

    m_produced.store(0);
    m_consumed.store(0);
    m_startTime = std::chrono::steady_clock::now();
    m_running.store(true);

    m_workers.clear();
    m_workers.push_back(std::make_unique<WorkerThread>([this] { runSource(); }));
    for (std::size_t i = 0; i < m_stages.size(); ++i) {
        if (m_stageQueues[i]) {
            const std::size_t index = i;
            m_workers.push_back(
                std::make_unique<WorkerThread>([this, index] { runStage(index); }));
        }
    }
    return true;
}

void Pipeline::closeQueues()
{
    for (auto &queue : m_stageQueues) {
        if (queue)
            queue->close();
    }
}

void Pipeline::joinWorkers()
{
    for (auto &worker : m_workers)
        worker->join();
    m_workers.clear();
}

void Pipeline::stop()
{
    m_running.store(false);




    closeQueues();
    joinWorkers();

    if (m_source && m_ownsSource)
        m_source->close();
    for (auto &stage : m_stages)
        stage->onStop();
}

void Pipeline::pushToNext(std::size_t stageIndex, labcore::FramePtr frame)
{
    if (!frame)
        return;
    if (stageIndex >= m_stages.size()) {
        m_consumed.fetch_add(1);
        if (m_output)
            m_output(std::move(frame));
        return;
    }
    if (m_stageQueues[stageIndex])
        m_stageQueues[stageIndex]->push(std::move(frame));
}

labcore::FramePtr Pipeline::runInlineChain(std::size_t startIndex, labcore::FramePtr frame,
                                           std::size_t *nextStageIndex)
{
    std::size_t i = startIndex;
    for (; i < m_stages.size(); ++i) {
        labcore::Stage *stage = m_stages[i].get();
        if (stage->execution() != labcore::Stage::Execution::Inline)
            break;
        StageCounter &counter = *m_counters[i];
        counter.in.fetch_add(1);
        const auto begin = std::chrono::steady_clock::now();
        frame = stage->process(frame);
        counter.totalUs.fetch_add(static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - begin).count()));
        if (frame)
            counter.out.fetch_add(1);
        else
            counter.dropped.fetch_add(1);
        if (!frame)
            break;
    }
    if (nextStageIndex)
        *nextStageIndex = i;
    return frame;
}

void Pipeline::runSource()
{
    while (m_running.load()) {
        labcore::FramePtr frame = m_source->next();
        if (!frame)
            break;
        m_produced.fetch_add(1);
        std::size_t next = m_stages.size();
        frame = runInlineChain(0, std::move(frame), &next);
        pushToNext(next, std::move(frame));
    }
    closeQueues();
    m_running.store(false);
}

void Pipeline::runStage(std::size_t stageIndex)
{
    labcore::BoundedQueue<labcore::FramePtr> &queue = *m_stageQueues[stageIndex];
    labcore::Stage *stage = m_stages[stageIndex].get();
    StageCounter &counter = *m_counters[stageIndex];

    labcore::FramePtr frame;
    while (queue.pop(frame)) {
        counter.in.fetch_add(1);
        const auto begin = std::chrono::steady_clock::now();
        labcore::FramePtr output = stage->process(frame);
        counter.totalUs.fetch_add(static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - begin).count()));
        if (!output) {
            counter.dropped.fetch_add(1);
            continue;
        }
        counter.out.fetch_add(1);
        std::size_t next = m_stages.size();
        output = runInlineChain(stageIndex + 1, std::move(output), &next);
        pushToNext(next, std::move(output));
    }
}

Pipeline::Stats Pipeline::stats() const
{
    Stats result;
    result.running = m_running.load();
    result.produced = m_produced.load();
    result.consumed = m_consumed.load();

    std::uint64_t dropped = 0;
    for (const auto &queue : m_stageQueues) {
        if (queue)
            dropped += queue->dropped();
    }

    result.stages.reserve(m_stages.size());
    for (std::size_t i = 0; i < m_stages.size(); ++i) {
        StageStat stat;
        stat.name = std::string(m_stages[i]->name());
        const StageCounter &counter = *m_counters[i];
        stat.in = counter.in.load();
        stat.out = counter.out.load();
        stat.dropped = counter.dropped.load();
        dropped += stat.dropped;
        const std::uint64_t in = stat.in;
        if (in > 0)
            stat.avgMs = static_cast<double>(counter.totalUs.load()) / static_cast<double>(in) / 1000.0;
        result.stages.push_back(std::move(stat));
    }
    result.dropped = dropped;

    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - m_startTime;
    if (elapsed.count() > 0.0)
        result.fps = static_cast<double>(result.consumed) / elapsed.count();
    return result;
}

}
