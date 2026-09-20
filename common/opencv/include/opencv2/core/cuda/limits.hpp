

#ifndef OPENCV_CUDA_LIMITS_HPP
#define OPENCV_CUDA_LIMITS_HPP

#include <limits.h>
#include <float.h>
#include "common.hpp"





namespace cv { namespace cuda { namespace device
{
template <class T> struct numeric_limits;

template <> struct numeric_limits<bool>
{
    __device__ __forceinline__ static bool min() { return false; }
    __device__ __forceinline__ static bool max() { return true;  }
    static const bool is_signed = false;
};

template <> struct numeric_limits<signed char>
{
    __device__ __forceinline__ static signed char min() { return SCHAR_MIN; }
    __device__ __forceinline__ static signed char max() { return SCHAR_MAX; }
    static const bool is_signed = true;
};

template <> struct numeric_limits<unsigned char>
{
    __device__ __forceinline__ static unsigned char min() { return 0; }
    __device__ __forceinline__ static unsigned char max() { return UCHAR_MAX; }
    static const bool is_signed = false;
};

template <> struct numeric_limits<short>
{
    __device__ __forceinline__ static short min() { return SHRT_MIN; }
    __device__ __forceinline__ static short max() { return SHRT_MAX; }
    static const bool is_signed = true;
};

template <> struct numeric_limits<unsigned short>
{
    __device__ __forceinline__ static unsigned short min() { return 0; }
    __device__ __forceinline__ static unsigned short max() { return USHRT_MAX; }
    static const bool is_signed = false;
};

template <> struct numeric_limits<int>
{
    __device__ __forceinline__ static int min() { return INT_MIN; }
    __device__ __forceinline__ static int max() { return INT_MAX; }
    static const bool is_signed = true;
};

template <> struct numeric_limits<unsigned int>
{
    __device__ __forceinline__ static unsigned int min() { return 0; }
    __device__ __forceinline__ static unsigned int max() { return UINT_MAX; }
    static const bool is_signed = false;
};

template <> struct numeric_limits<float>
{
    __device__ __forceinline__ static float min() { return FLT_MIN; }
    __device__ __forceinline__ static float max() { return FLT_MAX; }
    __device__ __forceinline__ static float epsilon() { return FLT_EPSILON; }
    static const bool is_signed = true;
};

template <> struct numeric_limits<double>
{
    __device__ __forceinline__ static double min() { return DBL_MIN; }
    __device__ __forceinline__ static double max() { return DBL_MAX; }
    __device__ __forceinline__ static double epsilon() { return DBL_EPSILON; }
    static const bool is_signed = true;
};
}}}



#endif
