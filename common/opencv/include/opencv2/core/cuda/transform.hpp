

#ifndef OPENCV_CUDA_TRANSFORM_HPP
#define OPENCV_CUDA_TRANSFORM_HPP

#include "common.hpp"
#include "utility.hpp"
#include "detail/transform_detail.hpp"





namespace cv { namespace cuda { namespace device
{
    template <typename T, typename D, typename UnOp, typename Mask>
    static inline void transform(PtrStepSz<T> src, PtrStepSz<D> dst, UnOp op, const Mask& mask, cudaStream_t stream)
    {
        typedef TransformFunctorTraits<UnOp> ft;
        transform_detail::TransformDispatcher<VecTraits<T>::cn == 1 && VecTraits<D>::cn == 1 && ft::smart_shift != 1>::call(src, dst, op, mask, stream);
    }

    template <typename T1, typename T2, typename D, typename BinOp, typename Mask>
    static inline void transform(PtrStepSz<T1> src1, PtrStepSz<T2> src2, PtrStepSz<D> dst, BinOp op, const Mask& mask, cudaStream_t stream)
    {
        typedef TransformFunctorTraits<BinOp> ft;
        transform_detail::TransformDispatcher<VecTraits<T1>::cn == 1 && VecTraits<T2>::cn == 1 && VecTraits<D>::cn == 1 && ft::smart_shift != 1>::call(src1, src2, dst, op, mask, stream);
    }
}}}



#endif
