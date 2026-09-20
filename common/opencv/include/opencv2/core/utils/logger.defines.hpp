



#ifndef OPENCV_LOGGER_DEFINES_HPP
#define OPENCV_LOGGER_DEFINES_HPP





#define CV_LOG_LEVEL_SILENT 0
#define CV_LOG_LEVEL_FATAL 1
#define CV_LOG_LEVEL_ERROR 2
#define CV_LOG_LEVEL_WARN 3
#define CV_LOG_LEVEL_INFO 4
#define CV_LOG_LEVEL_DEBUG 5
#define CV_LOG_LEVEL_VERBOSE 6

namespace cv {
namespace utils {
namespace logging {


enum LogLevel {
    LOG_LEVEL_SILENT = 0,
    LOG_LEVEL_FATAL = 1,
    LOG_LEVEL_ERROR = 2,
    LOG_LEVEL_WARNING = 3,
    LOG_LEVEL_INFO = 4,
    LOG_LEVEL_DEBUG = 5,
    LOG_LEVEL_VERBOSE = 6,
#ifndef CV_DOXYGEN
    ENUM_LOG_LEVEL_FORCE_INT = INT_MAX
#endif
};

}}}



#endif
