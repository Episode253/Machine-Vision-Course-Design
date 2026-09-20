

#ifndef OPENCV_CUDA_COMMON_HPP
#define OPENCV_CUDA_COMMON_HPP

#include <cuda_runtime.h>
#include "opencv2/core/cuda_types.hpp"
#include "opencv2/core/cvdef.h"
#include "opencv2/core/base.hpp"





#ifndef CV_PI_F
    #ifndef CV_PI
        #define CV_PI_F 3.14159265f
    #else
        #define CV_PI_F ((float)CV_PI)
    #endif
#endif

namespace cv { namespace cuda {
    static inline void checkCudaError(cudaError_t err, const char* file, const int line, const char* func)
    {
        if (cudaSuccess != err)
            cv::error(cv::Error::GpuApiCallError, cudaGetErrorString(err), func, file, line);
    }
}}

#ifndef cudaSafeCall
    #define cudaSafeCall(expr)  cv::cuda::checkCudaError(expr, __FILE__, __LINE__, CV_Func)
#endif

namespace cv { namespace cuda
{
    template <typename T> static inline bool isAligned(const T* ptr, size_t size)
    {
        return reinterpret_cast<size_t>(ptr) % size == 0;
    }

    static inline bool isAligned(size_t step, size_t size)
    {
        return step % size == 0;
    }
}}

namespace cv { namespace cuda
{
    namespace device
    {
        __host__ __device__ __forceinline__ int divUp(int total, int grain)
        {
            return (total + grain - 1) / grain;
        }

        template<class T> inline void bindTexture(const textureReference* tex, const PtrStepSz<T>& img)
        {
            cudaChannelFormatDesc desc = cudaCreateChannelDesc<T>();
            cudaSafeCall( cudaBindTexture2D(0, tex, img.ptr(), &desc, img.cols, img.rows, img.step) );
        }
    }
}}



#endif
