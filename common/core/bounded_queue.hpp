#ifndef VISUAL_LAB_CORE_BOUNDED_QUEUE_H
#define VISUAL_LAB_CORE_BOUNDED_QUEUE_H

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>
#include <utility>

namespace labcore {



enum class OverflowPolicy {
    Block,
    DropOldest,
    DropNewest
};

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity,
                          OverflowPolicy policy = OverflowPolicy::Block)
        : m_capacity(capacity == 0 ? 1 : capacity), m_policy(policy)
    {
    }

    BoundedQueue(const BoundedQueue &) = delete;
    BoundedQueue &operator=(const BoundedQueue &) = delete;


    bool push(T value)
    {
        // unique_lock 用于条件变量等待：wait 会临时释放锁，唤醒后再自动重新加锁。
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_closed)
            return false;
        while (m_items.size() >= m_capacity) {
            if (m_policy == OverflowPolicy::DropOldest) {
                m_items.pop_front();
                ++m_dropped;
                break;
            }
            if (m_policy == OverflowPolicy::DropNewest) {
                ++m_dropped;
                return true;
            }
            m_notFull.wait(lock, [this] {
                return m_closed || m_items.size() < m_capacity;
            });
            if (m_closed)
                return false;
        }
        m_items.push_back(std::move(value));
        m_notEmpty.notify_one();
        return true;
    }



    bool pop(T &out)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_notEmpty.wait(lock, [this] { return m_closed || !m_items.empty(); });
        if (m_items.empty())
            return false;
        out = std::move(m_items.front());
        m_items.pop_front();
        m_notFull.notify_one();
        return true;
    }

    bool tryPop(T &out)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_items.empty())
            return false;
        out = std::move(m_items.front());
        m_items.pop_front();
        m_notFull.notify_one();
        return true;
    }


    void close()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_closed = true;
        }
        // 关闭是所有消费者的终止信号，必须同时唤醒“等数据”和“等空位”的线程。
        m_notEmpty.notify_all();
        m_notFull.notify_all();
    }

    bool closed() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_closed;
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_items.size();
    }

    std::size_t capacity() const { return m_capacity; }

    std::uint64_t dropped() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_dropped;
    }

private:
    mutable std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
    std::deque<T> m_items;
    const std::size_t m_capacity;
    const OverflowPolicy m_policy;
    bool m_closed = false;
    std::uint64_t m_dropped = 0;
};

}

#endif
