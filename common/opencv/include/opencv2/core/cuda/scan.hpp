

#ifndef OPENCV_CUDA_SCAN_HPP
#define OPENCV_CUDA_SCAN_HPP

#include "opencv2/core/cuda/common.hpp"
#include "opencv2/core/cuda/utility.hpp"
#include "opencv2/core/cuda/warp.hpp"
#include "opencv2/core/cuda/warp_shuffle.hpp"





namespace cv { namespace cuda { namespace device
{
    enum ScanKind { EXCLUSIVE = 0,  INCLUSIVE = 1 };

    template <ScanKind Kind, typename T, typename F> struct WarpScan
    {
        __device__ __forceinline__ WarpScan() {}
        __device__ __forceinline__ WarpScan(const WarpScan& other) { CV_UNUSED(other); }

        __device__ __forceinline__ T operator()( volatile T *ptr , const unsigned int idx)
        {
            const unsigned int lane = idx & 31;
            F op;

            if ( lane >=  1) ptr [idx ] = op(ptr [idx -  1], ptr [idx]);
            if ( lane >=  2) ptr [idx ] = op(ptr [idx -  2], ptr [idx]);
            if ( lane >=  4) ptr [idx ] = op(ptr [idx -  4], ptr [idx]);
            if ( lane >=  8) ptr [idx ] = op(ptr [idx -  8], ptr [idx]);
            if ( lane >= 16) ptr [idx ] = op(ptr [idx - 16], ptr [idx]);

            if( Kind == INCLUSIVE )
                return ptr [idx];
            else
                return (lane > 0) ? ptr [idx - 1] : 0;
        }

        __device__ __forceinline__ unsigned int index(const unsigned int tid)
        {
            return tid;
        }

        __device__ __forceinline__ void init(volatile T *ptr){}

        static const int warp_offset      = 0;

        typedef WarpScan<INCLUSIVE, T, F>  merge;
    };

    template <ScanKind Kind , typename T, typename F> struct WarpScanNoComp
    {
        __device__ __forceinline__ WarpScanNoComp() {}
        __device__ __forceinline__ WarpScanNoComp(const WarpScanNoComp& other) { CV_UNUSED(other); }

        __device__ __forceinline__ T operator()( volatile T *ptr , const unsigned int idx)
        {
            const unsigned int lane = threadIdx.x & 31;
            F op;

            ptr [idx ] = op(ptr [idx -  1], ptr [idx]);
            ptr [idx ] = op(ptr [idx -  2], ptr [idx]);
            ptr [idx ] = op(ptr [idx -  4], ptr [idx]);
            ptr [idx ] = op(ptr [idx -  8], ptr [idx]);
            ptr [idx ] = op(ptr [idx - 16], ptr [idx]);

            if( Kind == INCLUSIVE )
                return ptr [idx];
            else
                return (lane > 0) ? ptr [idx - 1] : 0;
        }

        __device__ __forceinline__ unsigned int index(const unsigned int tid)
        {
            return (tid >> warp_log) * warp_smem_stride + 16 + (tid & warp_mask);
        }

        __device__ __forceinline__ void init(volatile T *ptr)
        {
            ptr[threadIdx.x] = 0;
        }

        static const int warp_smem_stride = 32 + 16 + 1;
        static const int warp_offset      = 16;
        static const int warp_log         = 5;
        static const int warp_mask        = 31;

        typedef WarpScanNoComp<INCLUSIVE, T, F> merge;
    };

    template <ScanKind Kind , typename T, typename Sc, typename F> struct BlockScan
    {
        __device__ __forceinline__ BlockScan() {}
        __device__ __forceinline__ BlockScan(const BlockScan& other) { CV_UNUSED(other); }

        __device__ __forceinline__ T operator()(volatile T *ptr)
        {
            const unsigned int tid  = threadIdx.x;
            const unsigned int lane = tid & warp_mask;
            const unsigned int warp = tid >> warp_log;

            Sc scan;
            typename Sc::merge merge_scan;
            const unsigned int idx = scan.index(tid);

            T val = scan(ptr, idx);
            __syncthreads ();

            if( warp == 0)
                scan.init(ptr);
            __syncthreads ();

            if( lane == 31 )
                ptr [scan.warp_offset + warp ] = (Kind == INCLUSIVE) ? val : ptr [idx];
            __syncthreads ();

            if( warp == 0 )
                merge_scan(ptr, idx);
            __syncthreads();

            if ( warp > 0)
                val = ptr [scan.warp_offset + warp - 1] + val;
            __syncthreads ();

            ptr[idx] = val;
            __syncthreads ();

            return val ;
        }

        static const int warp_log  = 5;
        static const int warp_mask = 31;
    };

    template <typename T>
    __device__ T warpScanInclusive(T idata, volatile T* s_Data, unsigned int tid)
    {
    #if __CUDA_ARCH__ >= 300
        const unsigned int laneId = cv::cuda::device::Warp::laneId();


        #pragma unroll
        for (int i = 1; i <= (OPENCV_CUDA_WARP_SIZE / 2); i *= 2)
        {
            const T n = cv::cuda::device::shfl_up(idata, i);
            if (laneId >= i)
                  idata += n;
        }

        return idata;
    #else
        unsigned int pos = 2 * tid - (tid & (OPENCV_CUDA_WARP_SIZE - 1));
        s_Data[pos] = 0;
        pos += OPENCV_CUDA_WARP_SIZE;
        s_Data[pos] = idata;

        s_Data[pos] += s_Data[pos - 1];
        s_Data[pos] += s_Data[pos - 2];
        s_Data[pos] += s_Data[pos - 4];
        s_Data[pos] += s_Data[pos - 8];
        s_Data[pos] += s_Data[pos - 16];

        return s_Data[pos];
    #endif
    }

    template <typename T>
    __device__ __forceinline__ T warpScanExclusive(T idata, volatile T* s_Data, unsigned int tid)
    {
        return warpScanInclusive(idata, s_Data, tid) - idata;
    }

    template <int tiNumScanThreads, typename T>
    __device__ T blockScanInclusive(T idata, volatile T* s_Data, unsigned int tid)
    {
        if (tiNumScanThreads > OPENCV_CUDA_WARP_SIZE)
        {

            T warpResult = warpScanInclusive(idata, s_Data, tid);



            __syncthreads();
            if ((tid & (OPENCV_CUDA_WARP_SIZE - 1)) == (OPENCV_CUDA_WARP_SIZE - 1))
            {
                s_Data[tid >> OPENCV_CUDA_LOG_WARP_SIZE] = warpResult;
            }


            __syncthreads();

            if (tid < (tiNumScanThreads / OPENCV_CUDA_WARP_SIZE) )
            {

                T val = s_Data[tid];

                s_Data[tid] = warpScanExclusive(val, s_Data, tid);
            }


            __syncthreads();

            return warpResult + s_Data[tid >> OPENCV_CUDA_LOG_WARP_SIZE];
        }
        else
        {
            return warpScanInclusive(idata, s_Data, tid);
        }
    }
}}}



#endif
