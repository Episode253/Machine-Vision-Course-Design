






#ifndef OPENCV_GAPI_GTYPE_TRAITS_HPP
#define OPENCV_GAPI_GTYPE_TRAITS_HPP

#include <vector>
#include <type_traits>

#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>
#include <opencv2/gapi/garray.hpp>
#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/own/convert.hpp>

namespace cv
{
namespace detail
{



    enum class ArgKind: int
    {
        OPAQUE,
        GOBJREF,
        GMAT,
        GMATP,
        GSCALAR,
        GARRAY,
    };





    template<typename T> struct GTypeTraits;
    template<typename T> struct GTypeTraits
    {
        static constexpr const ArgKind kind = ArgKind::OPAQUE;
    };
    template<>           struct GTypeTraits<cv::GMat>
    {
        static constexpr const ArgKind kind = ArgKind::GMAT;
        static constexpr const GShape shape = GShape::GMAT;
    };
    template<>           struct GTypeTraits<cv::GMatP>
    {
        static constexpr const ArgKind kind = ArgKind::GMATP;
        static constexpr const GShape shape = GShape::GMAT;
    };
    template<>           struct GTypeTraits<cv::GScalar>
    {
        static constexpr const ArgKind kind = ArgKind::GSCALAR;
        static constexpr const GShape shape = GShape::GSCALAR;
    };
    template<class T> struct GTypeTraits<cv::GArray<T> >
    {
        static constexpr const ArgKind kind = ArgKind::GARRAY;
        static constexpr const GShape shape = GShape::GARRAY;
        using host_type  = std::vector<T>;
        using strip_type = cv::detail::VectorRef;
        static cv::detail::GArrayU   wrap_value(const cv::GArray<T>  &t) { return t.strip();}
        static cv::detail::VectorRef wrap_in   (const std::vector<T> &t) { return detail::VectorRef(t); }
        static cv::detail::VectorRef wrap_out  (      std::vector<T> &t) { return detail::VectorRef(t); }
    };



    template<class T> struct has_custom_wrap
    {
        template<class,class> class check;
        template<typename C> static std::true_type  test(check<C, decltype(&GTypeTraits<C>::wrap_value)> *);
        template<typename C> static std::false_type test(...);
        using type = decltype(test<T>(nullptr));
        static const constexpr bool value = std::is_same<std::true_type, decltype(test<T>(nullptr))>::value;
    };







    template<typename T> struct GTypeOf;
#if !defined(GAPI_STANDALONE)
    template<>           struct GTypeOf<cv::Mat>               { using type = cv::GMat;      };
    template<>           struct GTypeOf<cv::UMat>              { using type = cv::GMat;      };
    template<>           struct GTypeOf<cv::Scalar>            { using type = cv::GScalar;   };
#endif
    template<>           struct GTypeOf<cv::gapi::own::Mat>    { using type = cv::GMat;      };
    template<>           struct GTypeOf<cv::gapi::own::Scalar> { using type = cv::GScalar;   };
    template<typename U> struct GTypeOf<std::vector<U> >       { using type = cv::GArray<U>; };
    template<class T> using g_type_of_t = typename GTypeOf<T>::type;































    template<typename T, class Custom = void> struct WrapValue
    {
        static auto wrap(const T& t) ->
            typename std::remove_reference<T>::type
        {
            return static_cast<typename std::remove_reference<T>::type>(t);
        }

        template<typename U> static U  wrap_in (const U &u) { return  u;  }
        template<typename U> static U* wrap_out(U &u)       { return &u;  }
    };
    template<typename T> struct WrapValue<T, typename std::enable_if<has_custom_wrap<T>::value>::type>
    {
        static auto wrap(const T& t) -> decltype(GTypeTraits<T>::wrap_value(t))
        {
            return GTypeTraits<T>::wrap_value(t);
        }
        template<typename U> static auto wrap_in (const U &u) -> typename GTypeTraits<T>::strip_type
        {
            return GTypeTraits<T>::wrap_in(u);
        }
        template<typename U> static auto wrap_out(U &u) -> typename GTypeTraits<T>::strip_type
        {
            return GTypeTraits<T>::wrap_out(u);
        }
    };

    template<typename T> using wrap_gapi_helper = WrapValue<typename std::decay<T>::type>;
    template<typename T> using wrap_host_helper = WrapValue<typename std::decay<g_type_of_t<T> >::type>;

}
}

#endif
