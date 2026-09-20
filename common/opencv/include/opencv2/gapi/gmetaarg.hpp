






#ifndef OPENCV_GAPI_GMETAARG_HPP
#define OPENCV_GAPI_GMETAARG_HPP

#include <vector>
#include <type_traits>

#include <opencv2/gapi/util/util.hpp>
#include <opencv2/gapi/util/variant.hpp>

#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>
#include <opencv2/gapi/garray.hpp>

namespace cv
{











using GMetaArg = util::variant
    < util::monostate
    , GMatDesc
    , GScalarDesc
    , GArrayDesc
    >;
GAPI_EXPORTS std::ostream& operator<<(std::ostream& os, const GMetaArg &);

using GMetaArgs = std::vector<GMetaArg>;

namespace detail
{





    template<typename T> struct is_meta_descr    : std::false_type {};
    template<> struct is_meta_descr<GMatDesc>    : std::true_type {};
    template<> struct is_meta_descr<GScalarDesc> : std::true_type {};
    template<> struct is_meta_descr<GArrayDesc>  : std::true_type {};

    template<typename... Ts>
    using are_meta_descrs = all_satisfy<is_meta_descr, Ts...>;

    template<typename... Ts>
    using are_meta_descrs_but_last = all_satisfy<is_meta_descr, typename all_but_last<Ts...>::type>;

}

class Mat;
class UMat;
GAPI_EXPORTS cv::GMetaArgs descr_of(const std::vector<cv::Mat> &vec);
GAPI_EXPORTS cv::GMetaArgs descr_of(const std::vector<cv::UMat> &vec);
namespace gapi { namespace own {
    class Mat;
    GAPI_EXPORTS cv::GMetaArgs descr_of(const std::vector<Mat> &vec);
}}

}

#endif
