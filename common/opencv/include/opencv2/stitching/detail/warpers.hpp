

#ifndef OPENCV_STITCHING_WARPERS_HPP
#define OPENCV_STITCHING_WARPERS_HPP

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv_modules.hpp"

namespace cv {
namespace detail {





class CV_EXPORTS RotationWarper
{
public:
    virtual ~RotationWarper() {}


    virtual Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R) = 0;


    virtual Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) = 0;


    virtual Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
                       CV_OUT OutputArray dst) = 0;


    virtual void warpBackward(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
                              Size dst_size, CV_OUT OutputArray dst) = 0;


    virtual Rect warpRoi(Size src_size, InputArray K, InputArray R) = 0;

    virtual float getScale() const { return 1.f; }
    virtual void setScale(float) {}
};


struct CV_EXPORTS_W_SIMPLE ProjectorBase
{
    void setCameraParams(InputArray K = Mat::eye(3, 3, CV_32F),
                         InputArray R = Mat::eye(3, 3, CV_32F),
                         InputArray T = Mat::zeros(3, 1, CV_32F));

    float scale;
    float k[9];
    float rinv[9];
    float r_kinv[9];
    float k_rinv[9];
    float t[3];
};


template <class P>
class CV_EXPORTS_TEMPLATE RotationWarperBase : public RotationWarper
{
public:
    Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R) CV_OVERRIDE;

    Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) CV_OVERRIDE;

    Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
               OutputArray dst) CV_OVERRIDE;

    void warpBackward(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
                      Size dst_size, OutputArray dst) CV_OVERRIDE;

    Rect warpRoi(Size src_size, InputArray K, InputArray R) CV_OVERRIDE;

    float getScale() const  CV_OVERRIDE{ return projector_.scale; }
    void setScale(float val) CV_OVERRIDE { projector_.scale = val; }

protected:


    virtual void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br);



    void detectResultRoiByBorder(Size src_size, Point &dst_tl, Point &dst_br);

    P projector_;
};


struct CV_EXPORTS PlaneProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS PlaneWarper : public RotationWarperBase<PlaneProjector>
{
public:

    PlaneWarper(float scale = 1.f) { projector_.scale = scale; }

    Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R) CV_OVERRIDE;
    Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R, InputArray T);

    virtual Rect buildMaps(Size src_size, InputArray K, InputArray R, InputArray T, CV_OUT OutputArray xmap, CV_OUT OutputArray ymap);
    Rect buildMaps(Size src_size, InputArray K, InputArray R, CV_OUT OutputArray xmap, CV_OUT OutputArray ymap) CV_OVERRIDE;

    Point warp(InputArray src, InputArray K, InputArray R,
               int interp_mode, int border_mode, CV_OUT OutputArray dst) CV_OVERRIDE;
    virtual Point warp(InputArray src, InputArray K, InputArray R, InputArray T, int interp_mode, int border_mode,
        CV_OUT OutputArray dst);

    Rect warpRoi(Size src_size, InputArray K, InputArray R) CV_OVERRIDE;
    Rect warpRoi(Size src_size, InputArray K, InputArray R, InputArray T);

protected:
    void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br) CV_OVERRIDE;
};



class CV_EXPORTS AffineWarper : public PlaneWarper
{
public:

    AffineWarper(float scale = 1.f) : PlaneWarper(scale) {}

    Point2f warpPoint(const Point2f &pt, InputArray K, InputArray R) CV_OVERRIDE;
    Rect buildMaps(Size src_size, InputArray K, InputArray R, CV_OUT OutputArray xmap, CV_OUT  OutputArray ymap) CV_OVERRIDE;
    Point warp(InputArray src, InputArray K, InputArray R,
               int interp_mode, int border_mode, CV_OUT OutputArray dst) CV_OVERRIDE;
    Rect warpRoi(Size src_size, InputArray K, InputArray R) CV_OVERRIDE;

protected:

    void getRTfromHomogeneous(InputArray H, Mat &R, Mat &T);
};


struct CV_EXPORTS_W_SIMPLE SphericalProjector : ProjectorBase
{
    CV_WRAP void mapForward(float x, float y, float &u, float &v);
    CV_WRAP void mapBackward(float u, float v, float &x, float &y);
};



class CV_EXPORTS SphericalWarper : public RotationWarperBase<SphericalProjector>
{
public:

    SphericalWarper(float scale) { projector_.scale = scale; }

    Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) CV_OVERRIDE;
    Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode, OutputArray dst) CV_OVERRIDE;
protected:
    void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br) CV_OVERRIDE;
};


struct CV_EXPORTS CylindricalProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};



class CV_EXPORTS CylindricalWarper : public RotationWarperBase<CylindricalProjector>
{
public:

    CylindricalWarper(float scale) { projector_.scale = scale; }

    Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) CV_OVERRIDE;
    Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode, OutputArray dst) CV_OVERRIDE;
protected:
    void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br) CV_OVERRIDE
    {
        RotationWarperBase<CylindricalProjector>::detectResultRoiByBorder(src_size, dst_tl, dst_br);
    }
};


struct CV_EXPORTS FisheyeProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS FisheyeWarper : public RotationWarperBase<FisheyeProjector>
{
public:
    FisheyeWarper(float scale) { projector_.scale = scale; }
};


struct CV_EXPORTS StereographicProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS StereographicWarper : public RotationWarperBase<StereographicProjector>
{
public:
    StereographicWarper(float scale) { projector_.scale = scale; }
};


struct CV_EXPORTS CompressedRectilinearProjector : ProjectorBase
{
    float a, b;

    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS CompressedRectilinearWarper : public RotationWarperBase<CompressedRectilinearProjector>
{
public:
    CompressedRectilinearWarper(float scale, float A = 1, float B = 1)
    {
        projector_.a = A;
        projector_.b = B;
        projector_.scale = scale;
    }
};


struct CV_EXPORTS CompressedRectilinearPortraitProjector : ProjectorBase
{
    float a, b;

    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS CompressedRectilinearPortraitWarper : public RotationWarperBase<CompressedRectilinearPortraitProjector>
{
public:
   CompressedRectilinearPortraitWarper(float scale, float A = 1, float B = 1)
   {
       projector_.a = A;
       projector_.b = B;
       projector_.scale = scale;
   }
};


struct CV_EXPORTS PaniniProjector : ProjectorBase
{
    float a, b;

    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS PaniniWarper : public RotationWarperBase<PaniniProjector>
{
public:
   PaniniWarper(float scale, float A = 1, float B = 1)
   {
       projector_.a = A;
       projector_.b = B;
       projector_.scale = scale;
   }
};


struct CV_EXPORTS PaniniPortraitProjector : ProjectorBase
{
    float a, b;

    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS PaniniPortraitWarper : public RotationWarperBase<PaniniPortraitProjector>
{
public:
   PaniniPortraitWarper(float scale, float A = 1, float B = 1)
   {
       projector_.a = A;
       projector_.b = B;
       projector_.scale = scale;
   }

};


struct CV_EXPORTS MercatorProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS MercatorWarper : public RotationWarperBase<MercatorProjector>
{
public:
    MercatorWarper(float scale) { projector_.scale = scale; }
};


struct CV_EXPORTS TransverseMercatorProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS TransverseMercatorWarper : public RotationWarperBase<TransverseMercatorProjector>
{
public:
    TransverseMercatorWarper(float scale) { projector_.scale = scale; }
};


class CV_EXPORTS PlaneWarperGpu : public PlaneWarper
{
public:
    PlaneWarperGpu(float scale = 1.f) : PlaneWarper(scale) {}

    Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) CV_OVERRIDE
    {
        Rect result = buildMaps(src_size, K, R, d_xmap_, d_ymap_);
        d_xmap_.download(xmap);
        d_ymap_.download(ymap);
        return result;
    }

    Rect buildMaps(Size src_size, InputArray K, InputArray R, InputArray T, OutputArray xmap, OutputArray ymap) CV_OVERRIDE
    {
        Rect result = buildMaps(src_size, K, R, T, d_xmap_, d_ymap_);
        d_xmap_.download(xmap);
        d_ymap_.download(ymap);
        return result;
    }

    Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
               OutputArray dst) CV_OVERRIDE
    {
        d_src_.upload(src);
        Point result = warp(d_src_, K, R, interp_mode, border_mode, d_dst_);
        d_dst_.download(dst);
        return result;
    }

    Point warp(InputArray src, InputArray K, InputArray R, InputArray T, int interp_mode, int border_mode,
               OutputArray dst) CV_OVERRIDE
    {
        d_src_.upload(src);
        Point result = warp(d_src_, K, R, T, interp_mode, border_mode, d_dst_);
        d_dst_.download(dst);
        return result;
    }

    Rect buildMaps(Size src_size, InputArray K, InputArray R, cuda::GpuMat & xmap, cuda::GpuMat & ymap);

    Rect buildMaps(Size src_size, InputArray K, InputArray R, InputArray T, cuda::GpuMat & xmap, cuda::GpuMat & ymap);

    Point warp(const cuda::GpuMat & src, InputArray K, InputArray R, int interp_mode, int border_mode,
               cuda::GpuMat & dst);

    Point warp(const cuda::GpuMat & src, InputArray K, InputArray R, InputArray T, int interp_mode, int border_mode,
               cuda::GpuMat & dst);

private:
    cuda::GpuMat d_xmap_, d_ymap_, d_src_, d_dst_;
};


class CV_EXPORTS SphericalWarperGpu : public SphericalWarper
{
public:
    SphericalWarperGpu(float scale) : SphericalWarper(scale) {}

    Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) CV_OVERRIDE
    {
        Rect result = buildMaps(src_size, K, R, d_xmap_, d_ymap_);
        d_xmap_.download(xmap);
        d_ymap_.download(ymap);
        return result;
    }

    Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
               OutputArray dst) CV_OVERRIDE
    {
        d_src_.upload(src);
        Point result = warp(d_src_, K, R, interp_mode, border_mode, d_dst_);
        d_dst_.download(dst);
        return result;
    }

    Rect buildMaps(Size src_size, InputArray K, InputArray R, cuda::GpuMat & xmap, cuda::GpuMat & ymap);

    Point warp(const cuda::GpuMat & src, InputArray K, InputArray R, int interp_mode, int border_mode,
               cuda::GpuMat & dst);

private:
    cuda::GpuMat d_xmap_, d_ymap_, d_src_, d_dst_;
};


class CV_EXPORTS CylindricalWarperGpu : public CylindricalWarper
{
public:
    CylindricalWarperGpu(float scale) : CylindricalWarper(scale) {}

    Rect buildMaps(Size src_size, InputArray K, InputArray R, OutputArray xmap, OutputArray ymap) CV_OVERRIDE
    {
        Rect result = buildMaps(src_size, K, R, d_xmap_, d_ymap_);
        d_xmap_.download(xmap);
        d_ymap_.download(ymap);
        return result;
    }

    Point warp(InputArray src, InputArray K, InputArray R, int interp_mode, int border_mode,
               OutputArray dst) CV_OVERRIDE
    {
        d_src_.upload(src);
        Point result = warp(d_src_, K, R, interp_mode, border_mode, d_dst_);
        d_dst_.download(dst);
        return result;
    }

    Rect buildMaps(Size src_size, InputArray K, InputArray R, cuda::GpuMat & xmap, cuda::GpuMat & ymap);

    Point warp(const cuda::GpuMat & src, InputArray K, InputArray R, int interp_mode, int border_mode,
               cuda::GpuMat & dst);

private:
    cuda::GpuMat d_xmap_, d_ymap_, d_src_, d_dst_;
};


struct CV_EXPORTS SphericalPortraitProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};




class CV_EXPORTS SphericalPortraitWarper : public RotationWarperBase<SphericalPortraitProjector>
{
public:
    SphericalPortraitWarper(float scale) { projector_.scale = scale; }

protected:
    void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br) CV_OVERRIDE;
};

struct CV_EXPORTS CylindricalPortraitProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS CylindricalPortraitWarper : public RotationWarperBase<CylindricalPortraitProjector>
{
public:
    CylindricalPortraitWarper(float scale) { projector_.scale = scale; }

protected:
    void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br) CV_OVERRIDE
    {
        RotationWarperBase<CylindricalPortraitProjector>::detectResultRoiByBorder(src_size, dst_tl, dst_br);
    }
};

struct CV_EXPORTS PlanePortraitProjector : ProjectorBase
{
    void mapForward(float x, float y, float &u, float &v);
    void mapBackward(float u, float v, float &x, float &y);
};


class CV_EXPORTS PlanePortraitWarper : public RotationWarperBase<PlanePortraitProjector>
{
public:
    PlanePortraitWarper(float scale) { projector_.scale = scale; }

protected:
    void detectResultRoi(Size src_size, Point &dst_tl, Point &dst_br) CV_OVERRIDE
    {
        RotationWarperBase<PlanePortraitProjector>::detectResultRoiByBorder(src_size, dst_tl, dst_br);
    }
};



}
}

#include "warpers_inl.hpp"

#endif
