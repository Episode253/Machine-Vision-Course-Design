

#ifndef OPENCV_CUDA_TYPE_TRAITS_HPP
#define OPENCV_CUDA_TYPE_TRAITS_HPP

#include "detail/type_traits_detail.hpp"





namespace cv { namespace cuda { namespace device
{
    template <typename T> struct IsSimpleParameter
    {
        enum {value = type_traits_detail::IsIntegral<T>::value || type_traits_detail::IsFloat<T>::value ||
            type_traits_detail::PointerTraits<typename type_traits_detail::ReferenceTraits<T>::type>::value};
    };

    template <typename T> struct TypeTraits
    {
        typedef typename type_traits_detail::UnConst<T>::type                                                NonConstType;
        typedef typename type_traits_detail::UnVolatile<T>::type                                             NonVolatileType;
        typedef typename type_traits_detail::UnVolatile<typename type_traits_detail::UnConst<T>::type>::type UnqualifiedType;
        typedef typename type_traits_detail::PointerTraits<UnqualifiedType>::type                            PointeeType;
        typedef typename type_traits_detail::ReferenceTraits<T>::type                                        ReferredType;

        enum { isConst          = type_traits_detail::UnConst<T>::value };
        enum { isVolatile       = type_traits_detail::UnVolatile<T>::value };

        enum { isReference      = type_traits_detail::ReferenceTraits<UnqualifiedType>::value };
        enum { isPointer        = type_traits_detail::PointerTraits<typename type_traits_detail::ReferenceTraits<UnqualifiedType>::type>::value };

        enum { isUnsignedInt    = type_traits_detail::IsUnsignedIntegral<UnqualifiedType>::value };
        enum { isSignedInt      = type_traits_detail::IsSignedIntergral<UnqualifiedType>::value };
        enum { isIntegral       = type_traits_detail::IsIntegral<UnqualifiedType>::value };
        enum { isFloat          = type_traits_detail::IsFloat<UnqualifiedType>::value };
        enum { isArith          = isIntegral || isFloat };
        enum { isVec            = type_traits_detail::IsVec<UnqualifiedType>::value };

        typedef typename type_traits_detail::Select<IsSimpleParameter<UnqualifiedType>::value,
            T, typename type_traits_detail::AddParameterType<T>::type>::type ParameterType;
    };
}}}



#endif
