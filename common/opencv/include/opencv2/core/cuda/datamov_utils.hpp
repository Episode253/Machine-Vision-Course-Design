

#ifndef OPENCV_CUDA_DATAMOV_UTILS_HPP
#define OPENCV_CUDA_DATAMOV_UTILS_HPP

#include "common.hpp"





namespace cv { namespace cuda { namespace device
{
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 200


        template <typename T> struct ForceGlob
        {
            __device__ __forceinline__ static void Load(const T* ptr, int offset, T& val)  { val = ptr[offset];  }
        };

    #else

        #if defined(_WIN64) || defined(__LP64__)

            #define OPENCV_CUDA_ASM_PTR "l"
        #else

            #define OPENCV_CUDA_ASM_PTR "r"
        #endif

        template<class T> struct ForceGlob;

        #define OPENCV_CUDA_DEFINE_FORCE_GLOB(base_type, ptx_type, reg_mod) \
            template <> struct ForceGlob<base_type> \
            { \
                __device__ __forceinline__ static void Load(const base_type* ptr, int offset, base_type& val) \
                { \
                    asm("ld.global."#ptx_type" %0, [%1];" : "="#reg_mod(val) : OPENCV_CUDA_ASM_PTR(ptr + offset)); \
                } \
            };

        #define OPENCV_CUDA_DEFINE_FORCE_GLOB_B(base_type, ptx_type) \
            template <> struct ForceGlob<base_type> \
            { \
                __device__ __forceinline__ static void Load(const base_type* ptr, int offset, base_type& val) \
                { \
                    asm("ld.global."#ptx_type" %0, [%1];" : "=r"(*reinterpret_cast<uint*>(&val)) : OPENCV_CUDA_ASM_PTR(ptr + offset)); \
                } \
            };

            OPENCV_CUDA_DEFINE_FORCE_GLOB_B(uchar,  u8)
            OPENCV_CUDA_DEFINE_FORCE_GLOB_B(schar,  s8)
            OPENCV_CUDA_DEFINE_FORCE_GLOB_B(char,   b8)
            OPENCV_CUDA_DEFINE_FORCE_GLOB  (ushort, u16, h)
            OPENCV_CUDA_DEFINE_FORCE_GLOB  (short,  s16, h)
            OPENCV_CUDA_DEFINE_FORCE_GLOB  (uint,   u32, r)
            OPENCV_CUDA_DEFINE_FORCE_GLOB  (int,    s32, r)
            OPENCV_CUDA_DEFINE_FORCE_GLOB  (float,  f32, f)
            OPENCV_CUDA_DEFINE_FORCE_GLOB  (double, f64, d)

        #undef OPENCV_CUDA_DEFINE_FORCE_GLOB
        #undef OPENCV_CUDA_DEFINE_FORCE_GLOB_B
        #undef OPENCV_CUDA_ASM_PTR

    #endif
}}}



#endif
