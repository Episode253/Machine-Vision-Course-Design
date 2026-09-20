



#ifndef OPENCV_CORE_ALLOCATOR_STATS_HPP
#define OPENCV_CORE_ALLOCATOR_STATS_HPP

#include "../cvdef.h"

namespace cv { namespace utils {

class AllocatorStatisticsInterface
{
protected:
    AllocatorStatisticsInterface() {}
    virtual ~AllocatorStatisticsInterface() {}
public:
    virtual uint64_t getCurrentUsage() const = 0;
    virtual uint64_t getTotalUsage() const = 0;
    virtual uint64_t getNumberOfAllocations() const = 0;
    virtual uint64_t getPeakUsage() const = 0;


    virtual void resetPeakUsage() = 0;
};

}}

#endif
