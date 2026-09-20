






#ifndef OPENCV_GAPI_GMAT_HPP
#define OPENCV_GAPI_GMAT_HPP

#include <ostream>
#include <memory>

#include <opencv2/gapi/opencv_includes.hpp>
#include <opencv2/gapi/gcommon.hpp>

#include <opencv2/gapi/own/types.hpp>
#include <opencv2/gapi/own/convert.hpp>
#include <opencv2/gapi/own/assert.hpp>


namespace cv
{


class GNode;
struct GOrigin;



class GAPI_EXPORTS GMat
{
public:
    GMat();
    GMat(const GNode &n, std::size_t out);

    GOrigin& priv();
    const GOrigin& priv()  const;

private:
    std::shared_ptr<GOrigin> m_priv;
};

class GAPI_EXPORTS GMatP : public GMat
{
public:
    using GMat::GMat;
};

namespace gapi { namespace own {
    class Mat;
}}




struct GAPI_EXPORTS GMatDesc
{

    int depth;
    int chan;
    cv::gapi::own::Size size;
    bool planar;

    GMatDesc(int d, int c, cv::gapi::own::Size s, bool p = false)
        : depth(d), chan(c), size(s), planar(p) {}

    GMatDesc() : GMatDesc(-1, -1, {-1,-1}) {}

    inline bool operator== (const GMatDesc &rhs) const
    {
        return depth == rhs.depth && chan == rhs.chan && size == rhs.size && planar == rhs.planar;
    }

    inline bool operator!= (const GMatDesc &rhs) const
    {
        return !(*this == rhs);
    }





    bool canDescribe(const cv::gapi::own::Mat& mat) const;




    GMatDesc withSizeDelta(cv::gapi::own::Size delta) const
    {
        GMatDesc desc(*this);
        desc.size += delta;
        return desc;
    }
#if !defined(GAPI_STANDALONE)
    GMatDesc withSizeDelta(cv::Size delta) const
    {
        return withSizeDelta(to_own(delta));
    }

    GMatDesc withSize(cv::Size sz) const
    {
        return withSize(to_own(sz));
    }

    bool canDescribe(const cv::Mat& mat) const;
#endif




    GMatDesc withSizeDelta(int dx, int dy) const
    {
        return withSizeDelta(cv::gapi::own::Size{dx,dy});
    }

    GMatDesc withSize(cv::gapi::own::Size sz) const
    {
        GMatDesc desc(*this);
        desc.size = sz;
        return desc;
    }



    GMatDesc withDepth(int ddepth) const
    {
        GAPI_Assert(CV_MAT_CN(ddepth) == 1 || ddepth == -1);
        GMatDesc desc(*this);
        if (ddepth != -1) desc.depth = ddepth;
        return desc;
    }




    GMatDesc withType(int ddepth, int dchan) const
    {
        GAPI_Assert(CV_MAT_CN(ddepth) == 1 || ddepth == -1);
        GMatDesc desc = withDepth(ddepth);
        desc.chan = dchan;
        return desc;
    }




    GMatDesc asPlanar() const
    {
        GAPI_Assert(planar == false);
        GMatDesc desc(*this);
        desc.planar = true;
        return desc;
    }




    GMatDesc asPlanar(int planes) const
    {
        GAPI_Assert(planar == false);
        GAPI_Assert(chan == 1);
        GAPI_Assert(planes > 1);
        GAPI_Assert(size.height % planes == 0);
        GMatDesc desc(*this);
        desc.size.height /=  planes;
        desc.chan = planes;
        return desc.asPlanar();
    }




    GMatDesc asInterleaved() const
    {
        GAPI_Assert(planar == true);
        GMatDesc desc(*this);
        desc.planar = false;
        return desc;
    }
};

static inline GMatDesc empty_gmat_desc() { return GMatDesc{-1,-1,{-1,-1}}; }

#if !defined(GAPI_STANDALONE)
class Mat;
GAPI_EXPORTS GMatDesc descr_of(const cv::Mat &mat);
GAPI_EXPORTS GMatDesc descr_of(const cv::UMat &mat);
#endif



namespace gapi { namespace own {
    GAPI_EXPORTS GMatDesc descr_of(const Mat &mat);
}}

GAPI_EXPORTS std::ostream& operator<<(std::ostream& os, const cv::GMatDesc &desc);

}

#endif
