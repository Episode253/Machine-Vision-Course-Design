

#ifndef OPENCV_CUDA_VEC_DISTANCE_DETAIL_HPP
#define OPENCV_CUDA_VEC_DISTANCE_DETAIL_HPP

#include "../datamov_utils.hpp"



namespace cv { namespace cuda { namespace device
{
    namespace vec_distance_detail
    {
        template <int THREAD_DIM, int N> struct UnrollVecDiffCached
        {
            template <typename Dist, typename T1, typename T2>
            static __device__ void calcCheck(const T1* vecCached, const T2* vecGlob, int len, Dist& dist, int ind)
            {
                if (ind < len)
                {
                    T1 val1 = *vecCached++;

                    T2 val2;
                    ForceGlob<T2>::Load(vecGlob, ind, val2);

                    dist.reduceIter(val1, val2);

                    UnrollVecDiffCached<THREAD_DIM, N - 1>::calcCheck(vecCached, vecGlob, len, dist, ind + THREAD_DIM);
                }
            }

            template <typename Dist, typename T1, typename T2>
            static __device__ void calcWithoutCheck(const T1* vecCached, const T2* vecGlob, Dist& dist)
            {
                T1 val1 = *vecCached++;

                T2 val2;
                ForceGlob<T2>::Load(vecGlob, 0, val2);
                vecGlob += THREAD_DIM;

                dist.reduceIter(val1, val2);

                UnrollVecDiffCached<THREAD_DIM, N - 1>::calcWithoutCheck(vecCached, vecGlob, dist);
            }
        };
        template <int THREAD_DIM> struct UnrollVecDiffCached<THREAD_DIM, 0>
        {
            template <typename Dist, typename T1, typename T2>
            static __device__ __forceinline__ void calcCheck(const T1*, const T2*, int, Dist&, int)
            {
            }

            template <typename Dist, typename T1, typename T2>
            static __device__ __forceinline__ void calcWithoutCheck(const T1*, const T2*, Dist&)
            {
            }
        };

        template <int THREAD_DIM, int MAX_LEN, bool LEN_EQ_MAX_LEN> struct VecDiffCachedCalculator;
        template <int THREAD_DIM, int MAX_LEN> struct VecDiffCachedCalculator<THREAD_DIM, MAX_LEN, false>
        {
            template <typename Dist, typename T1, typename T2>
            static __device__ __forceinline__ void calc(const T1* vecCached, const T2* vecGlob, int len, Dist& dist, int tid)
            {
                UnrollVecDiffCached<THREAD_DIM, MAX_LEN / THREAD_DIM>::calcCheck(vecCached, vecGlob, len, dist, tid);
            }
        };
        template <int THREAD_DIM, int MAX_LEN> struct VecDiffCachedCalculator<THREAD_DIM, MAX_LEN, true>
        {
            template <typename Dist, typename T1, typename T2>
            static __device__ __forceinline__ void calc(const T1* vecCached, const T2* vecGlob, int len, Dist& dist, int tid)
            {
                UnrollVecDiffCached<THREAD_DIM, MAX_LEN / THREAD_DIM>::calcWithoutCheck(vecCached, vecGlob + tid, dist);
            }
        };
    }
}}}



#endif
