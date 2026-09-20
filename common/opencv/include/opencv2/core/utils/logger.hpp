



#ifndef OPENCV_LOGGER_HPP
#define OPENCV_LOGGER_HPP

#include <iostream>
#include <sstream>
#include <limits.h>

#include "logger.defines.hpp"
#include "logtag.hpp"






namespace cv {
namespace utils {
namespace logging {


CV_EXPORTS LogLevel setLogLevel(LogLevel logLevel);

CV_EXPORTS LogLevel getLogLevel();

CV_EXPORTS void registerLogTag(cv::utils::logging::LogTag* plogtag);

CV_EXPORTS void setLogTagLevel(const char* tag, cv::utils::logging::LogLevel level);

CV_EXPORTS cv::utils::logging::LogLevel getLogTagLevel(const char* tag);

namespace internal {


CV_EXPORTS cv::utils::logging::LogTag* getGlobalLogTag();


CV_EXPORTS void writeLogMessage(LogLevel logLevel, const char* message);


CV_EXPORTS void writeLogMessageEx(LogLevel logLevel, const char* tag, const char* file, int line, const char* func, const char* message);

}

struct LogTagAuto
    : public LogTag
{
    inline LogTagAuto(const char* _name, LogLevel _level)
        : LogTag(_name, _level)
    {
        registerLogTag(this);
    }
};


#ifndef CV_LOG_STRIP_LEVEL
# if defined NDEBUG
#   define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG
# else
#   define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE
# endif
#endif

#define CV_LOGTAG_PTR_CAST(expr) static_cast<const cv::utils::logging::LogTag*>(expr)





















#define CV_LOGTAG_EXPAND_NAME(tag) tag





#define CV_LOGTAG_FALLBACK nullptr





#define CV_LOGTAG_GLOBAL cv::utils::logging::internal::getGlobalLogTag()

#define CV_LOG_WITH_TAG(tag, msgLevel, ...) \
    for(;;) { \
        const auto cv_temp_msglevel = (cv::utils::logging::LogLevel)(msgLevel); \
        if (cv_temp_msglevel >= (CV_LOG_STRIP_LEVEL)) break; \
        auto cv_temp_logtagptr = CV_LOGTAG_PTR_CAST(CV_LOGTAG_EXPAND_NAME(tag)); \
        if (!cv_temp_logtagptr) cv_temp_logtagptr = CV_LOGTAG_PTR_CAST(CV_LOGTAG_FALLBACK); \
        if (!cv_temp_logtagptr) cv_temp_logtagptr = CV_LOGTAG_PTR_CAST(CV_LOGTAG_GLOBAL); \
        if (cv_temp_logtagptr && (cv_temp_msglevel > cv_temp_logtagptr->level)) break; \
        std::stringstream cv_temp_logstream; \
        cv_temp_logstream << __VA_ARGS__; \
        cv::utils::logging::internal::writeLogMessageEx( \
            cv_temp_msglevel, \
            (cv_temp_logtagptr ? cv_temp_logtagptr->name : nullptr), \
            __FILE__, \
            __LINE__, \
            CV_Func, \
            cv_temp_logstream.str().c_str()); \
        break; \
    }

#define CV_LOG_FATAL(tag, ...)   CV_LOG_WITH_TAG(tag, cv::utils::logging::LOG_LEVEL_FATAL, __VA_ARGS__)
#define CV_LOG_ERROR(tag, ...)   CV_LOG_WITH_TAG(tag, cv::utils::logging::LOG_LEVEL_ERROR, __VA_ARGS__)
#define CV_LOG_WARNING(tag, ...) CV_LOG_WITH_TAG(tag, cv::utils::logging::LOG_LEVEL_WARNING, __VA_ARGS__)
#define CV_LOG_INFO(tag, ...) CV_LOG_WITH_TAG(tag, cv::utils::logging::LOG_LEVEL_INFO, __VA_ARGS__)
#define CV_LOG_DEBUG(tag, ...) CV_LOG_WITH_TAG(tag, cv::utils::logging::LOG_LEVEL_DEBUG, __VA_ARGS__)
#define CV_LOG_VERBOSE(tag, v, ...) CV_LOG_WITH_TAG(tag, (cv::utils::logging::LOG_LEVEL_VERBOSE + (int)(v)), __VA_ARGS__)

#if CV_LOG_STRIP_LEVEL <= CV_LOG_LEVEL_INFO
# undef CV_LOG_INFO
# define CV_LOG_INFO(tag, ...)
#endif

#if CV_LOG_STRIP_LEVEL <= CV_LOG_LEVEL_DEBUG
# undef CV_LOG_DEBUG
# define CV_LOG_DEBUG(tag, ...)
#endif

#if CV_LOG_STRIP_LEVEL <= CV_LOG_LEVEL_VERBOSE
# undef CV_LOG_VERBOSE
# define CV_LOG_VERBOSE(tag, v, ...)
#endif

}}}



#endif
