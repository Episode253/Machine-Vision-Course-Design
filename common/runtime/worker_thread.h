#ifndef VISUAL_LAB_RUNTIME_WORKER_THREAD_H
#define VISUAL_LAB_RUNTIME_WORKER_THREAD_H

#include <functional>
#include <thread>
#include <utility>

namespace labruntime {



class WorkerThread {
public:
    WorkerThread() = default;

    explicit WorkerThread(std::function<void()> task)
    {
        start(std::move(task));
    }

    ~WorkerThread()
    {
        // RAII 收尾：析构前 join，避免 std::thread 仍可连接而触发程序终止。
        join();
    }

    WorkerThread(WorkerThread &&other) noexcept
        : m_thread(std::move(other.m_thread))
    {
    }

    WorkerThread &operator=(WorkerThread &&other) noexcept
    {
        if (this != &other) {
            join();
            m_thread = std::move(other.m_thread);
        }
        return *this;
    }

    WorkerThread(const WorkerThread &) = delete;
    WorkerThread &operator=(const WorkerThread &) = delete;

    void start(std::function<void()> task)
    {
        join();
        m_thread = std::thread(std::move(task));
    }

    void join()
    {
        if (m_thread.joinable())
            m_thread.join();
    }

    bool joinable() const { return m_thread.joinable(); }

private:
    std::thread m_thread;
};

}

#endif
