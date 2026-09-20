

#ifndef OPENCV_FLANN_LOGGER_H
#define OPENCV_FLANN_LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#include "defines.h"


namespace cvflann
{

class Logger
{
    Logger() : stream(stdout), logLevel(FLANN_LOG_WARN) {}

    ~Logger()
    {
        if ((stream!=NULL)&&(stream!=stdout)) {
            fclose(stream);
        }
    }

    static Logger& instance()
    {
        static Logger logger;
        return logger;
    }

    void _setDestination(const char* name)
    {
        if (name==NULL) {
            stream = stdout;
        }
        else {
#ifdef _MSC_VER
            if (fopen_s(&stream, name, "w") != 0)
                stream = NULL;
#else
            stream = fopen(name,"w");
#endif
            if (stream == NULL) {
                stream = stdout;
            }
        }
    }

    int _log(int level, const char* fmt, va_list arglist)
    {
        if (level > logLevel ) return -1;
        int ret = vfprintf(stream, fmt, arglist);
        return ret;
    }

public:

    static void setLevel(int level) { instance().logLevel = level; }


    static void setDestination(const char* name) { instance()._setDestination(name); }


    static int log(int level, const char* fmt, ...)
    {
        va_list arglist;
        va_start(arglist, fmt);
        int ret = instance()._log(level,fmt,arglist);
        va_end(arglist);
        return ret;
    }

#define LOG_METHOD(NAME,LEVEL) \
    static int NAME(const char* fmt, ...) \
    { \
        va_list ap; \
        va_start(ap, fmt); \
        int ret = instance()._log(LEVEL, fmt, ap); \
        va_end(ap); \
        return ret; \
    }

    LOG_METHOD(fatal, FLANN_LOG_FATAL)
    LOG_METHOD(error, FLANN_LOG_ERROR)
    LOG_METHOD(warn, FLANN_LOG_WARN)
    LOG_METHOD(info, FLANN_LOG_INFO)

private:
    FILE* stream;
    int logLevel;
};

}

#endif
