



#ifndef OPENCV_CORE_ALLOCATOR_STATS_IMPL_HPP
#define OPENCV_CORE_ALLOCATOR_STATS_IMPL_HPP

#include "./allocator_stats.hpp"

#ifdef CV_CXX11
#include <atomic>
#endif

namespace cv { namespace utils {

#ifdef CV__ALLOCATOR_STATS_LOG
namespace {
#endif

class AllocatorStatistics : public AllocatorStatisticsInterface
{
protected:
#ifdef CV_CXX11
    std::atomic<long long> curr, total, total_allocs, peak;
#else
    volatile long long curr, total, total_allocs, peak;
#endif

public:
    AllocatorStatistics()
#ifndef CV_CXX11
        : curr(0), total(0), total_allocs(0), peak(0)
#endif
    {}
    ~AllocatorStatistics() CV_OVERRIDE {}



#ifdef CV_CXX11
    uint64_t getCurrentUsage() const CV_OVERRIDE { return (uint64_t)curr.load(); }
    uint64_t getTotalUsage() const CV_OVERRIDE { return (uint64_t)total.load(); }
    uint64_t getNumberOfAllocations() const CV_OVERRIDE { return (uint64_t)total_allocs.load(); }
    uint64_t getPeakUsage() const CV_OVERRIDE { return (uint64_t)peak.load(); }


    void resetPeakUsage() CV_OVERRIDE { peak.store(curr.load()); }


    void onAllocate(size_t sz)
    {
#ifdef CV__ALLOCATOR_STATS_LOG
        CV__ALLOCATOR_STATS_LOG(cv::format("allocate: %lld (curr=%lld)", (long long int)sz, (long long int)curr.load()));
#endif

        long long new_curr = curr.fetch_add((long long)sz) + (long long)sz;


        auto prev_peak = peak.load();
        while (prev_peak < new_curr)
        {
            if (peak.compare_exchange_weak(prev_peak, new_curr))
                break;
        }


        total += (long long)sz;
        total_allocs++;
    }
    void onFree(size_t sz)
    {
#ifdef CV__ALLOCATOR_STATS_LOG
        CV__ALLOCATOR_STATS_LOG(cv::format("free: %lld (curr=%lld)", (long long int)sz, (long long int)curr.load()));
#endif
        curr -= (long long)sz;
    }

#else
    uint64_t getCurrentUsage() const CV_OVERRIDE { return (uint64_t)curr; }
    uint64_t getTotalUsage() const CV_OVERRIDE { return (uint64_t)total; }
    uint64_t getNumberOfAllocations() const CV_OVERRIDE { return (uint64_t)total_allocs; }
    uint64_t getPeakUsage() const CV_OVERRIDE { return (uint64_t)peak; }

    void resetPeakUsage() CV_OVERRIDE { peak = curr; }


    void onAllocate(size_t sz)
    {
#ifdef CV__ALLOCATOR_STATS_LOG
        CV__ALLOCATOR_STATS_LOG(cv::format("allocate: %lld (curr=%lld)", (long long int)sz, (long long int)curr));
#endif

        uint64_t new_curr = (uint64_t)CV_XADD(&curr, (uint64_t)sz) + sz;

        peak = std::max((uint64_t)peak, new_curr);


        total += sz;

        CV_XADD(&total_allocs, (uint64_t)1);
    }
    void onFree(size_t sz)
    {
#ifdef CV__ALLOCATOR_STATS_LOG
        CV__ALLOCATOR_STATS_LOG(cv::format("free: %lld (curr=%lld)", (long long int)sz, (long long int)curr));
#endif
        CV_XADD(&curr, (uint64_t)-sz);
    }
#endif
};

#ifdef CV__ALLOCATOR_STATS_LOG
}
#endif

}}

#endif
