

#ifndef OPENCV_STITCHING_WARPER_CREATORS_HPP
#define OPENCV_STITCHING_WARPER_CREATORS_HPP

#include "opencv2/stitching/detail/warpers.hpp"
#include <string>

namespace cv {
    class CV_EXPORTS_W PyRotationWarper
    {
        Ptr<detail::RotationWarper> rw;

    public:
        CV_WRAP PyRotationWarper(String type, float scale);
        CV_WRAP PyRotationWarper() {};
        ~PyRotationWarper() {}


        CV_WRAP Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R);


        CV_WRAP Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap);


        CV_WRAP Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
            CV_OUT OutputArray dst);


        CV_WRAP void warpBackward(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
            Size dst_size, CV_OUT OutputArray dst);


        CV_WRAP Rect warpRoi(Size src_size, InputArray K, InputArray R);

        CV_WRAP float getScale() const { return 1.f; }
        CV_WRAP void setScale(float) {}
    };






class CV_EXPORTS_W WarperCreator
{
public:
    CV_WRAP virtual ~WarperCreator() {}
    virtual Ptr<detail::RotationWarper> create(float scale) const = 0;
};



class CV_EXPORTS  PlaneWarper : public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::PlaneWarper>(scale); }
};


class CV_EXPORTS  AffineWarper : public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::AffineWarper>(scale); }
};


class CV_EXPORTS CylindricalWarper: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::CylindricalWarper>(scale); }
};


class CV_EXPORTS SphericalWarper: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::SphericalWarper>(scale); }
};

class CV_EXPORTS FisheyeWarper : public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::FisheyeWarper>(scale); }
};

class CV_EXPORTS StereographicWarper: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::StereographicWarper>(scale); }
};

class CV_EXPORTS CompressedRectilinearWarper: public WarperCreator
{
    float a, b;
public:
    CompressedRectilinearWarper(float A = 1, float B = 1)
    {
        a = A; b = B;
    }
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::CompressedRectilinearWarper>(scale, a, b); }
};

class CV_EXPORTS CompressedRectilinearPortraitWarper: public WarperCreator
{
    float a, b;
public:
    CompressedRectilinearPortraitWarper(float A = 1, float B = 1)
    {
        a = A; b = B;
    }
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::CompressedRectilinearPortraitWarper>(scale, a, b); }
};

class CV_EXPORTS PaniniWarper: public WarperCreator
{
    float a, b;
public:
    PaniniWarper(float A = 1, float B = 1)
    {
        a = A; b = B;
    }
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::PaniniWarper>(scale, a, b); }
};

class CV_EXPORTS PaniniPortraitWarper: public WarperCreator
{
    float a, b;
public:
    PaniniPortraitWarper(float A = 1, float B = 1)
    {
        a = A; b = B;
    }
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::PaniniPortraitWarper>(scale, a, b); }
};

class CV_EXPORTS MercatorWarper: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::MercatorWarper>(scale); }
};

class CV_EXPORTS TransverseMercatorWarper: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::TransverseMercatorWarper>(scale); }
};



#ifdef HAVE_OPENCV_CUDAWARPING
class PlaneWarperGpu: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::PlaneWarperGpu>(scale); }
};


class CylindricalWarperGpu: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::CylindricalWarperGpu>(scale); }
};


class SphericalWarperGpu: public WarperCreator
{
public:
    Ptr<detail::RotationWarper> create(float scale) const CV_OVERRIDE { return makePtr<detail::SphericalWarperGpu>(scale); }
};
#endif



}

#endif
