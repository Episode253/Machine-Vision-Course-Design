

#ifndef OPENCV_CUDA_VEC_DISTANCE_HPP
#define OPENCV_CUDA_VEC_DISTANCE_HPP

#include "reduce.hpp"
#include "functional.hpp"
#include "detail/vec_distance_detail.hpp"





namespace cv { namespace cuda { namespace device
{
    template <typename T> struct L1Dist
    {
        typedef int value_type;
        typedef int result_type;

        __device__ __forceinline__ L1Dist() : mySum(0) {}

        __device__ __forceinline__ void reduceIter(int val1, int val2)
        {
            mySum = __sad(val1, val2, mySum);
        }

        template <int THREAD_DIM> __device__ __forceinline__ void reduceAll(int* smem, int tid)
        {
            reduce<THREAD_DIM>(smem, mySum, tid, plus<int>());
        }

        __device__ __forceinline__ operator int() const
        {
            return mySum;
        }

        int mySum;
    };
    template <> struct L1Dist<float>
    {
        typedef float value_type;
        typedef float result_type;

        __device__ __forceinline__ L1Dist() : mySum(0.0f) {}

        __device__ __forceinline__ void reduceIter(float val1, float val2)
        {
            mySum += ::fabs(val1 - val2);
        }

        template <int THREAD_DIM> __device__ __forceinline__ void reduceAll(float* smem, int tid)
        {
            reduce<THREAD_DIM>(smem, mySum, tid, plus<float>());
        }

        __device__ __forceinline__ operator float() const
        {
            return mySum;
        }

        float mySum;
    };

    struct L2Dist
    {
        typedef float value_type;
        typedef float result_type;

        __device__ __forceinline__ L2Dist() : mySum(0.0f) {}

        __device__ __forceinline__ void reduceIter(float val1, float val2)
        {
            float reg = val1 - val2;
            mySum += reg * reg;
        }

        template <int THREAD_DIM> __device__ __forceinline__ void reduceAll(float* smem, int tid)
        {
            reduce<THREAD_DIM>(smem, mySum, tid, plus<float>());
        }

        __device__ __forceinline__ operator float() const
        {
            return sqrtf(mySum);
        }

        float mySum;
    };

    struct HammingDist
    {
        typedef int value_type;
        typedef int result_type;

        __device__ __forceinline__ HammingDist() : mySum(0) {}

        __device__ __forceinline__ void reduceIter(int val1, int val2)
        {
            mySum += __popc(val1 ^ val2);
        }

        template <int THREAD_DIM> __device__ __forceinline__ void reduceAll(int* smem, int tid)
        {
            reduce<THREAD_DIM>(smem, mySum, tid, plus<int>());
        }

        __device__ __forceinline__ operator int() const
        {
            return mySum;
        }

        int mySum;
    };


    template <int THREAD_DIM, typename Dist, typename T1, typename T2>
    __device__ void calcVecDiffGlobal(const T1* vec1, const T2* vec2, int len, Dist& dist, typename Dist::result_type* smem, int tid)
    {
        for (int i = tid; i < len; i += THREAD_DIM)
        {
            T1 val1;
            ForceGlob<T1>::Load(vec1, i, val1);

            T2 val2;
            ForceGlob<T2>::Load(vec2, i, val2);

            dist.reduceIter(val1, val2);
        }

        dist.reduceAll<THREAD_DIM>(smem, tid);
    }


    template <int THREAD_DIM, int MAX_LEN, bool LEN_EQ_MAX_LEN, typename Dist, typename T1, typename T2>
    __device__ __forceinline__ void calcVecDiffCached(const T1* vecCached, const T2* vecGlob, int len, Dist& dist, typename Dist::result_type* smem, int tid)
    {
        vec_distance_detail::VecDiffCachedCalculator<THREAD_DIM, MAX_LEN, LEN_EQ_MAX_LEN>::calc(vecCached, vecGlob, len, dist, tid);

        dist.reduceAll<THREAD_DIM>(smem, tid);
    }


    template <int THREAD_DIM, typename T1> struct VecDiffGlobal
    {
        explicit __device__ __forceinline__ VecDiffGlobal(const T1* vec1_, int = 0, void* = 0, int = 0, int = 0)
        {
            vec1 = vec1_;
        }

        template <typename T2, typename Dist>
        __device__ __forceinline__ void calc(const T2* vec2, int len, Dist& dist, typename Dist::result_type* smem, int tid) const
        {
            calcVecDiffGlobal<THREAD_DIM>(vec1, vec2, len, dist, smem, tid);
        }

        const T1* vec1;
    };


    template <int THREAD_DIM, int MAX_LEN, bool LEN_EQ_MAX_LEN, typename U> struct VecDiffCachedRegister
    {
        template <typename T1> __device__ __forceinline__ VecDiffCachedRegister(const T1* vec1, int len, U* smem, int glob_tid, int tid)
        {
            if (glob_tid < len)
                smem[glob_tid] = vec1[glob_tid];
            __syncthreads();

            U* vec1ValsPtr = vec1Vals;

            #pragma unroll
            for (int i = tid; i < MAX_LEN; i += THREAD_DIM)
                *vec1ValsPtr++ = smem[i];

            __syncthreads();
        }

        template <typename T2, typename Dist>
        __device__ __forceinline__ void calc(const T2* vec2, int len, Dist& dist, typename Dist::result_type* smem, int tid) const
        {
            calcVecDiffCached<THREAD_DIM, MAX_LEN, LEN_EQ_MAX_LEN>(vec1Vals, vec2, len, dist, smem, tid);
        }

        U vec1Vals[MAX_LEN / THREAD_DIM];
    };
}}}



#endif
