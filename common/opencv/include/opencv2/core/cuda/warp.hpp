

#ifndef OPENCV_CUDA_DEVICE_WARP_HPP
#define OPENCV_CUDA_DEVICE_WARP_HPP





namespace cv { namespace cuda { namespace device
{
    struct Warp
    {
        enum
        {
            LOG_WARP_SIZE = 5,
            WARP_SIZE     = 1 << LOG_WARP_SIZE,
            STRIDE        = WARP_SIZE
        };


        static __device__ __forceinline__ unsigned int laneId()
        {
            unsigned int ret;
            asm("mov.u32 %0, %%laneid;" : "=r"(ret) );
            return ret;
        }

        template<typename It, typename T>
        static __device__ __forceinline__ void fill(It beg, It end, const T& value)
        {
            for(It t = beg + laneId(); t < end; t += STRIDE)
                *t = value;
        }

        template<typename InIt, typename OutIt>
        static __device__ __forceinline__ OutIt copy(InIt beg, InIt end, OutIt out)
        {
            for(InIt t = beg + laneId(); t < end; t += STRIDE, out += STRIDE)
                *out = *t;
            return out;
        }

        template<typename InIt, typename OutIt, class UnOp>
        static __device__ __forceinline__ OutIt transform(InIt beg, InIt end, OutIt out, UnOp op)
        {
            for(InIt t = beg + laneId(); t < end; t += STRIDE, out += STRIDE)
                *out = op(*t);
            return out;
        }

        template<typename InIt1, typename InIt2, typename OutIt, class BinOp>
        static __device__ __forceinline__ OutIt transform(InIt1 beg1, InIt1 end1, InIt2 beg2, OutIt out, BinOp op)
        {
            unsigned int lane = laneId();

            InIt1 t1 = beg1 + lane;
            InIt2 t2 = beg2 + lane;
            for(; t1 < end1; t1 += STRIDE, t2 += STRIDE, out += STRIDE)
                *out = op(*t1, *t2);
            return out;
        }

        template <class T, class BinOp>
        static __device__ __forceinline__ T reduce(volatile T *ptr, BinOp op)
        {
            const unsigned int lane = laneId();

            if (lane < 16)
            {
                T partial = ptr[lane];

                ptr[lane] = partial = op(partial, ptr[lane + 16]);
                ptr[lane] = partial = op(partial, ptr[lane + 8]);
                ptr[lane] = partial = op(partial, ptr[lane + 4]);
                ptr[lane] = partial = op(partial, ptr[lane + 2]);
                ptr[lane] = partial = op(partial, ptr[lane + 1]);
            }

            return *ptr;
        }

        template<typename OutIt, typename T>
        static __device__ __forceinline__ void yota(OutIt beg, OutIt end, T value)
        {
            unsigned int lane = laneId();
            value += lane;

            for(OutIt t = beg + lane; t < end; t += STRIDE, value += STRIDE)
                *t = value;
        }
    };
}}}



#endif
