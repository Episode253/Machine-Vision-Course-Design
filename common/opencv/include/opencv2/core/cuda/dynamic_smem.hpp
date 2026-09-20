

#ifndef OPENCV_CUDA_DYNAMIC_SMEM_HPP
#define OPENCV_CUDA_DYNAMIC_SMEM_HPP





namespace cv { namespace cuda { namespace device
{
    template<class T> struct DynamicSharedMem
    {
        __device__ __forceinline__ operator T*()
        {
            extern __shared__ int __smem[];
            return (T*)__smem;
        }

        __device__ __forceinline__ operator const T*() const
        {
            extern __shared__ int __smem[];
            return (T*)__smem;
        }
    };


    template<> struct DynamicSharedMem<double>
    {
        __device__ __forceinline__ operator double*()
        {
            extern __shared__ double __smem_d[];
            return (double*)__smem_d;
        }

        __device__ __forceinline__ operator const double*() const
        {
            extern __shared__ double __smem_d[];
            return (double*)__smem_d;
        }
    };
}}}



#endif
