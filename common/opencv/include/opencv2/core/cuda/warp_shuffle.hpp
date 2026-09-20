

#ifndef OPENCV_CUDA_WARP_SHUFFLE_HPP
#define OPENCV_CUDA_WARP_SHUFFLE_HPP





namespace cv { namespace cuda { namespace device
{
#if __CUDACC_VER_MAJOR__ >= 9
#  define __shfl(x, y, z) __shfl_sync(0xFFFFFFFFU, x, y, z)
#  define __shfl_up(x, y, z) __shfl_up_sync(0xFFFFFFFFU, x, y, z)
#  define __shfl_down(x, y, z) __shfl_down_sync(0xFFFFFFFFU, x, y, z)
#endif
    template <typename T>
    __device__ __forceinline__ T shfl(T val, int srcLane, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        return __shfl(val, srcLane, width);
    #else
        return T();
    #endif
    }
    __device__ __forceinline__ unsigned int shfl(unsigned int val, int srcLane, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        return (unsigned int) __shfl((int) val, srcLane, width);
    #else
        return 0;
    #endif
    }
    __device__ __forceinline__ double shfl(double val, int srcLane, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        int lo = __double2loint(val);
        int hi = __double2hiint(val);

        lo = __shfl(lo, srcLane, width);
        hi = __shfl(hi, srcLane, width);

        return __hiloint2double(hi, lo);
    #else
        return 0.0;
    #endif
    }

    template <typename T>
    __device__ __forceinline__ T shfl_down(T val, unsigned int delta, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        return __shfl_down(val, delta, width);
    #else
        return T();
    #endif
    }
    __device__ __forceinline__ unsigned int shfl_down(unsigned int val, unsigned int delta, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        return (unsigned int) __shfl_down((int) val, delta, width);
    #else
        return 0;
    #endif
    }
    __device__ __forceinline__ double shfl_down(double val, unsigned int delta, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        int lo = __double2loint(val);
        int hi = __double2hiint(val);

        lo = __shfl_down(lo, delta, width);
        hi = __shfl_down(hi, delta, width);

        return __hiloint2double(hi, lo);
    #else
        return 0.0;
    #endif
    }

    template <typename T>
    __device__ __forceinline__ T shfl_up(T val, unsigned int delta, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        return __shfl_up(val, delta, width);
    #else
        return T();
    #endif
    }
    __device__ __forceinline__ unsigned int shfl_up(unsigned int val, unsigned int delta, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        return (unsigned int) __shfl_up((int) val, delta, width);
    #else
        return 0;
    #endif
    }
    __device__ __forceinline__ double shfl_up(double val, unsigned int delta, int width = warpSize)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 300
        int lo = __double2loint(val);
        int hi = __double2hiint(val);

        lo = __shfl_up(lo, delta, width);
        hi = __shfl_up(hi, delta, width);

        return __hiloint2double(hi, lo);
    #else
        return 0.0;
    #endif
    }
}}}

#  undef __shfl
#  undef __shfl_up
#  undef __shfl_down



#endif
