






#ifndef OPENCV_GAPI_OWN_SATURATE_HPP
#define OPENCV_GAPI_OWN_SATURATE_HPP

#include <cmath>

#include <limits>
#include <type_traits>

#include <opencv2/gapi/own/assert.hpp>

namespace cv { namespace gapi { namespace own {






template<typename DST, typename SRC>
static inline DST saturate(SRC x)
{

    GAPI_DbgAssert(std::is_integral<DST>::value &&
                   std::is_integral<SRC>::value);

    if (std::is_same<DST, SRC>::value)
        return static_cast<DST>(x);

    if (sizeof(DST) > sizeof(SRC))
        return static_cast<DST>(x);




    return x < std::numeric_limits<DST>::min()?
               std::numeric_limits<DST>::min():
           x > std::numeric_limits<DST>::max()?
               std::numeric_limits<DST>::max():
           static_cast<DST>(x);
}




template<typename DST, typename SRC, typename R>
static inline DST saturate(SRC x, R round)
{
    if (std::is_floating_point<DST>::value)
    {
        return static_cast<DST>(x);
    }
    else if (std::is_integral<SRC>::value)
    {
        GAPI_DbgAssert(std::is_integral<DST>::value &&
                       std::is_integral<SRC>::value);
        return saturate<DST>(x);
    }
    else
    {
        GAPI_DbgAssert(std::is_integral<DST>::value &&
                 std::is_floating_point<SRC>::value);
#ifdef _WIN32


#pragma warning(disable: 4244)
#endif
        int ix = static_cast<int>(round(x));
#ifdef _WIN32
#pragma warning(default: 4244)
#endif
        return saturate<DST>(ix);
    }
}


inline double  ceild(double x) { return std::ceil(x); }
inline double floord(double x) { return std::floor(x); }
inline double roundd(double x) { return std::round(x); }
inline double  rintd(double x) { return std::rint(x); }

}
}
}
#endif
