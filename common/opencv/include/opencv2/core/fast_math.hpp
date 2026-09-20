

#ifndef OPENCV_CORE_FAST_MATH_HPP
#define OPENCV_CORE_FAST_MATH_HPP

#include "opencv2/core/cvdef.h"

#if ((defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ \
    && defined __SSE2__ && !defined __APPLE__)) && !defined(__CUDACC__)
#include <emmintrin.h>
#endif







#ifdef __cplusplus
#  include <cmath>
#else
#  ifdef __BORLANDC__
#    include <fastmath.h>
#  else
#    include <math.h>
#  endif
#endif

#if defined __GNUC__ && defined __arm__ && (defined __ARM_PCS_VFP || defined __ARM_VFPV3__ || defined __ARM_NEON__) && !defined __SOFTFP__ && !defined(__CUDACC__)

    #define ARM_ROUND(_value, _asm_string) \
        int res; \
        float temp; \
        CV_UNUSED(temp); \
        __asm__(_asm_string : [res] "=r" (res), [temp] "=w" (temp) : [value] "w" (_value)); \
        return res

    #ifdef __clang__
        #define ARM_ROUND_DBL(value) ARM_ROUND(value, "vcvtr.s32.f64 %[temp], %[value] \n vmov %[res], %[temp]")
    #else
        #define ARM_ROUND_DBL(value) ARM_ROUND(value, "vcvtr.s32.f64 %[temp], %P[value] \n vmov %[res], %[temp]")
    #endif

    #define ARM_ROUND_FLT(value) ARM_ROUND(value, "vcvtr.s32.f32 %[temp], %[value]\n vmov %[res], %[temp]")
#endif


CV_INLINE int
cvRound( double value )
{
#if ((defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ \
    && defined __SSE2__ && !defined __APPLE__) || CV_SSE2) && !defined(__CUDACC__)
    __m128d t = _mm_set_sd( value );
    return _mm_cvtsd_si32(t);
#elif defined _MSC_VER && defined _M_IX86
    int t;
    __asm
    {
        fld value;
        fistp t;
    }
    return t;
#elif defined CV_ICC || defined __GNUC__
# if defined ARM_ROUND_DBL
    ARM_ROUND_DBL(value);
# else
    return (int)lrint(value);
# endif
#else

    return (int)(value + (value >= 0 ? 0.5 : -0.5));
#endif
}



CV_INLINE int cvFloor( double value )
{
    int i = (int)value;
    return i - (i > value);
}


CV_INLINE int cvCeil( double value )
{
    int i = (int)value;
    return i + (i < value);
}


CV_INLINE int cvIsNaN( double value )
{
    Cv64suf ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) +
           ((unsigned)ieee754.u != 0) > 0x7ff00000;
}


CV_INLINE int cvIsInf( double value )
{
    Cv64suf ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
            (unsigned)ieee754.u == 0;
}

#ifdef __cplusplus


CV_INLINE int cvRound(float value)
{
#if ((defined _MSC_VER && defined _M_X64) || (defined __GNUC__ && defined __x86_64__ \
    && defined __SSE2__ && !defined __APPLE__) || CV_SSE2) && !defined(__CUDACC__)
    __m128 t = _mm_set_ss( value );
    return _mm_cvtss_si32(t);
#elif defined _MSC_VER && defined _M_IX86
    int t;
    __asm
    {
        fld value;
        fistp t;
    }
    return t;
#elif defined CV_ICC || defined __GNUC__
# if defined ARM_ROUND_FLT
    ARM_ROUND_FLT(value);
# else
    return (int)lrintf(value);
# endif
#else

    return (int)(value + (value >= 0 ? 0.5f : -0.5f));
#endif
}


CV_INLINE int cvRound( int value )
{
    return value;
}


CV_INLINE int cvFloor( float value )
{
    int i = (int)value;
    return i - (i > value);
}


CV_INLINE int cvFloor( int value )
{
    return value;
}


CV_INLINE int cvCeil( float value )
{
    int i = (int)value;
    return i + (i < value);
}


CV_INLINE int cvCeil( int value )
{
    return value;
}


CV_INLINE int cvIsNaN( float value )
{
    Cv32suf ieee754;
    ieee754.f = value;
    return (ieee754.u & 0x7fffffff) > 0x7f800000;
}


CV_INLINE int cvIsInf( float value )
{
    Cv32suf ieee754;
    ieee754.f = value;
    return (ieee754.u & 0x7fffffff) == 0x7f800000;
}

#endif



#endif
