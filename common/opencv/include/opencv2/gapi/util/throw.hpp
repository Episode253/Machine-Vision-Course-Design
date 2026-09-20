






#ifndef OPENCV_GAPI_UTIL_THROW_HPP
#define OPENCV_GAPI_UTIL_THROW_HPP

#include <utility>

#if !defined(__EXCEPTIONS)
#include <stdlib.h>
#include <stdio.h>
#endif

namespace cv
{
namespace util
{
template <class ExceptionType>
[[noreturn]] void throw_error(ExceptionType &&e)
{
#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
    throw std::forward<ExceptionType>(e);
#else
    fprintf(stderr, "An exception thrown! %s\n" , e.what());
    fflush(stderr);
    abort();
#endif
}
}
}

#endif
