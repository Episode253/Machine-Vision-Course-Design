

#ifndef OPENCV_CUDA_DEVICE_FUNCATTRIB_HPP
#define OPENCV_CUDA_DEVICE_FUNCATTRIB_HPP

#include <cstdio>





namespace cv { namespace cuda { namespace device
{
    template<class Func>
    void printFuncAttrib(Func& func)
    {

        cudaFuncAttributes attrs;
        cudaFuncGetAttributes(&attrs, func);

        printf("=== Function stats ===\n");
        printf("Name: \n");
        printf("sharedSizeBytes    = %d\n", attrs.sharedSizeBytes);
        printf("constSizeBytes     = %d\n", attrs.constSizeBytes);
        printf("localSizeBytes     = %d\n", attrs.localSizeBytes);
        printf("maxThreadsPerBlock = %d\n", attrs.maxThreadsPerBlock);
        printf("numRegs            = %d\n", attrs.numRegs);
        printf("ptxVersion         = %d\n", attrs.ptxVersion);
        printf("binaryVersion      = %d\n", attrs.binaryVersion);
        printf("\n");
        fflush(stdout);
    }
}}}



#endif
