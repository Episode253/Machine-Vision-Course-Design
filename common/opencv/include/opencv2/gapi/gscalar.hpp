







#ifndef OPENCV_GAPI_GSCALAR_HPP
#define OPENCV_GAPI_GSCALAR_HPP

#include <ostream>

#include <opencv2/gapi/opencv_includes.hpp>
#include <opencv2/gapi/gcommon.hpp>
#include <opencv2/gapi/util/optional.hpp>
#include <opencv2/gapi/own/scalar.hpp>

namespace cv
{


class GNode;
struct GOrigin;



class GAPI_EXPORTS GScalar
{
public:
    GScalar();
    explicit GScalar(const cv::gapi::own::Scalar& s);
    explicit GScalar(cv::gapi::own::Scalar&& s);
#if !defined(GAPI_STANDALONE)
    explicit GScalar(const cv::Scalar& s);
#endif
    GScalar(double v0);
    GScalar(const GNode &n, std::size_t out);

    GOrigin& priv();
    const GOrigin& priv()  const;

private:
    std::shared_ptr<GOrigin> m_priv;
};




struct GScalarDesc
{


    inline bool operator== (const GScalarDesc &) const
    {
        return true;
    }

    inline bool operator!= (const GScalarDesc &rhs) const
    {
        return !(*this == rhs);
    }
};

static inline GScalarDesc empty_scalar_desc() { return GScalarDesc(); }

#if !defined(GAPI_STANDALONE)
GAPI_EXPORTS GScalarDesc descr_of(const cv::Scalar            &scalar);
#endif


GAPI_EXPORTS GScalarDesc descr_of(const cv::gapi::own::Scalar &scalar);

std::ostream& operator<<(std::ostream& os, const cv::GScalarDesc &desc);

}

#endif
