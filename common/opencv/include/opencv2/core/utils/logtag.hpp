



#ifndef OPENCV_CORE_LOGTAG_HPP
#define OPENCV_CORE_LOGTAG_HPP

#include "opencv2/core/cvstd.hpp"
#include "logger.defines.hpp"

namespace cv {
namespace utils {
namespace logging {

struct LogTag
{
    const char* name;
    LogLevel level;

    inline LogTag(const char* _name, LogLevel _level)
        : name(_name)
        , level(_level)
    {}
};

}}}

#endif
