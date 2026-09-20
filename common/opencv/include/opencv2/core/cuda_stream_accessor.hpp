

#ifndef OPENCV_CORE_CUDA_STREAM_ACCESSOR_HPP
#define OPENCV_CORE_CUDA_STREAM_ACCESSOR_HPP

#ifndef __cplusplus
#  error cuda_stream_accessor.hpp header must be compiled as C++
#endif



#include <cuda_runtime.h>
#include "opencv2/core/cuda.hpp"

namespace cv
{
    namespace cuda
    {





        struct StreamAccessor
        {
            CV_EXPORTS static cudaStream_t getStream(const Stream& stream);
            CV_EXPORTS static Stream wrapStream(cudaStream_t stream);
        };


        struct EventAccessor
        {
            CV_EXPORTS static cudaEvent_t getEvent(const Event& event);
            CV_EXPORTS static Event wrapEvent(cudaEvent_t event);
        };



    }
}

#endif
