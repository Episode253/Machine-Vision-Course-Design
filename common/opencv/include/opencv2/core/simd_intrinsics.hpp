



#ifndef OPENCV_CORE_SIMD_INTRINSICS_HPP
#define OPENCV_CORE_SIMD_INTRINSICS_HPP



#ifdef __OPENCV_BUILD
#error "Use core/hal/intrin.hpp during OpenCV build"
#endif

#ifdef OPENCV_HAL_INTRIN_HPP
#error "core/simd_intrinsics.hpp must be included before core/hal/intrin.hpp"
#endif

#include "opencv2/core/cvdef.h"
#include "opencv2/core/version.hpp"

#ifdef OPENCV_SIMD_CONFIG_HEADER
#include CVAUX_STR(OPENCV_SIMD_CONFIG_HEADER)
#elif defined(OPENCV_SIMD_CONFIG_INCLUDE_DIR)
#include "opencv_simd_config.hpp"
#else

#if (!defined(CV_AVX_512F) || !CV_AVX_512F) && (defined(__AVX512__) || defined(__AVX512F__))
#  include <immintrin.h>
#  undef CV_AVX_512F
#  define CV_AVX_512F 1
#  ifndef OPENCV_SIMD_DONT_ASSUME_SKX
#    undef CV_AVX512_SKX
#    define CV_AVX512_SKX 1
#    undef CV_AVX_512CD
#    define CV_AVX_512CD 1
#    undef CV_AVX_512BW
#    define CV_AVX_512BW 1
#    undef CV_AVX_512DQ
#    define CV_AVX_512DQ 1
#    undef CV_AVX_512VL
#    define CV_AVX_512VL 1
#  endif
#endif



#if defined __AVX2__
#  include <immintrin.h>
#  undef CV_AVX2
#  define CV_AVX2 1
#  if defined __F16C__
#    undef CV_FP16
#    define CV_FP16 1
#  endif
#endif

#endif


#include "cv_cpu_dispatch.h"

#include "hal/intrin.hpp"

#endif
