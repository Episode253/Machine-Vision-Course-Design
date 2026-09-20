

#ifndef OPENCV_STITCHING_BLENDERS_HPP
#define OPENCV_STITCHING_BLENDERS_HPP

#if defined(NO)
#  warning Detected Apple 'NO' macro definition, it can cause build conflicts. Please, include this header before any Apple headers.
#endif

#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"

namespace cv {
namespace detail {





class CV_EXPORTS_W Blender
{
public:
    virtual ~Blender() {}

    enum { NO, FEATHER, MULTI_BAND };
    CV_WRAP static Ptr<Blender> createDefault(int type, bool try_gpu = false);


    CV_WRAP virtual void prepare(const std::vector<Point> &corners, const std::vector<Size> &sizes);

    CV_WRAP virtual void prepare(Rect dst_roi);

    CV_WRAP virtual void feed(InputArray img, InputArray mask, Point tl);

    CV_WRAP virtual void blend(CV_IN_OUT InputOutputArray dst,CV_IN_OUT  InputOutputArray dst_mask);

protected:
    UMat dst_, dst_mask_;
    Rect dst_roi_;
};


class CV_EXPORTS_W FeatherBlender : public Blender
{
public:
    CV_WRAP FeatherBlender(float sharpness = 0.02f);

    CV_WRAP float sharpness() const { return sharpness_; }
    CV_WRAP void setSharpness(float val) { sharpness_ = val; }

    CV_WRAP void prepare(Rect dst_roi) CV_OVERRIDE;
    CV_WRAP void feed(InputArray img, InputArray mask, Point tl) CV_OVERRIDE;
    CV_WRAP void blend(InputOutputArray dst, InputOutputArray dst_mask) CV_OVERRIDE;



    CV_WRAP Rect createWeightMaps(const std::vector<UMat> &masks, const std::vector<Point> &corners,
        CV_IN_OUT std::vector<UMat> &weight_maps);

private:
    float sharpness_;
    UMat weight_map_;
    UMat dst_weight_map_;
};

inline FeatherBlender::FeatherBlender(float _sharpness) { setSharpness(_sharpness); }


class CV_EXPORTS_W MultiBandBlender : public Blender
{
public:
    CV_WRAP MultiBandBlender(int try_gpu = false, int num_bands = 5, int weight_type = CV_32F);

    CV_WRAP int numBands() const { return actual_num_bands_; }
    CV_WRAP void setNumBands(int val) { actual_num_bands_ = val; }

    CV_WRAP void prepare(Rect dst_roi) CV_OVERRIDE;
    CV_WRAP void feed(InputArray img, InputArray mask, Point tl) CV_OVERRIDE;
    CV_WRAP void blend(CV_IN_OUT InputOutputArray dst, CV_IN_OUT InputOutputArray dst_mask) CV_OVERRIDE;

private:
    int actual_num_bands_, num_bands_;
    std::vector<UMat> dst_pyr_laplace_;
    std::vector<UMat> dst_band_weights_;
    Rect dst_roi_final_;
    bool can_use_gpu_;
    int weight_type_;
#if defined(HAVE_OPENCV_CUDAARITHM) && defined(HAVE_OPENCV_CUDAWARPING)
    std::vector<cuda::GpuMat> gpu_dst_pyr_laplace_;
    std::vector<cuda::GpuMat> gpu_dst_band_weights_;
    std::vector<Point> gpu_tl_points_;
    std::vector<cuda::GpuMat> gpu_imgs_with_border_;
    std::vector<std::vector<cuda::GpuMat> > gpu_weight_pyr_gauss_vec_;
    std::vector<std::vector<cuda::GpuMat> > gpu_src_pyr_laplace_vec_;
    std::vector<std::vector<cuda::GpuMat> > gpu_ups_;
    cuda::GpuMat gpu_dst_mask_;
    cuda::GpuMat gpu_mask_;
    cuda::GpuMat gpu_img_;
    cuda::GpuMat gpu_weight_map_;
    cuda::GpuMat gpu_add_mask_;
    int gpu_feed_idx_;
    bool gpu_initialized_;
#endif
};





void CV_EXPORTS_W normalizeUsingWeightMap(InputArray weight, CV_IN_OUT InputOutputArray src);

void CV_EXPORTS_W createWeightMap(InputArray mask, float sharpness, CV_IN_OUT InputOutputArray weight);

void CV_EXPORTS_W createLaplacePyr(InputArray img, int num_levels, CV_IN_OUT std::vector<UMat>& pyr);
void CV_EXPORTS_W createLaplacePyrGpu(InputArray img, int num_levels, CV_IN_OUT std::vector<UMat>& pyr);


void CV_EXPORTS_W restoreImageFromLaplacePyr(CV_IN_OUT std::vector<UMat>& pyr);
void CV_EXPORTS_W restoreImageFromLaplacePyrGpu(CV_IN_OUT std::vector<UMat>& pyr);



}
}

#endif
