

#ifndef OPENCV_STITCHING_STITCHER_HPP
#define OPENCV_STITCHING_STITCHER_HPP

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/stitching/warpers.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/camera.hpp"


#if defined(Status)
#  warning Detected X11 'Status' macro definition, it can cause build conflicts. Please, include this header before any X11 headers.
#endif




namespace cv {











class CV_EXPORTS_W Stitcher
{
public:

#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    static constexpr double ORIG_RESOL = -1.0;
#else

    static const double ORIG_RESOL;
#endif

    enum Status
    {
        OK = 0,
        ERR_NEED_MORE_IMGS = 1,
        ERR_HOMOGRAPHY_EST_FAIL = 2,
        ERR_CAMERA_PARAMS_ADJUST_FAIL = 3
    };

    enum Mode
    {

        PANORAMA = 0,

        SCANS = 1,

    };


    CV_WRAP static Ptr<Stitcher> create(Mode mode = Stitcher::PANORAMA);

    CV_WRAP double registrationResol() const { return registr_resol_; }
    CV_WRAP void setRegistrationResol(double resol_mpx) { registr_resol_ = resol_mpx; }

    CV_WRAP double seamEstimationResol() const { return seam_est_resol_; }
    CV_WRAP void setSeamEstimationResol(double resol_mpx) { seam_est_resol_ = resol_mpx; }

    CV_WRAP double compositingResol() const { return compose_resol_; }
    CV_WRAP void setCompositingResol(double resol_mpx) { compose_resol_ = resol_mpx; }

    CV_WRAP double panoConfidenceThresh() const { return conf_thresh_; }
    CV_WRAP void setPanoConfidenceThresh(double conf_thresh) { conf_thresh_ = conf_thresh; }

    CV_WRAP bool waveCorrection() const { return do_wave_correct_; }
    CV_WRAP void setWaveCorrection(bool flag) { do_wave_correct_ = flag; }

    CV_WRAP InterpolationFlags interpolationFlags() const { return interp_flags_; }
    CV_WRAP void setInterpolationFlags(InterpolationFlags interp_flags) { interp_flags_ = interp_flags; }

    detail::WaveCorrectKind waveCorrectKind() const { return wave_correct_kind_; }
    void setWaveCorrectKind(detail::WaveCorrectKind kind) { wave_correct_kind_ = kind; }

    Ptr<Feature2D> featuresFinder() { return features_finder_; }
    const Ptr<Feature2D> featuresFinder() const { return features_finder_; }
    void setFeaturesFinder(Ptr<Feature2D> features_finder)
        { features_finder_ = features_finder; }

    Ptr<detail::FeaturesMatcher> featuresMatcher() { return features_matcher_; }
    const Ptr<detail::FeaturesMatcher> featuresMatcher() const { return features_matcher_; }
    void setFeaturesMatcher(Ptr<detail::FeaturesMatcher> features_matcher)
        { features_matcher_ = features_matcher; }

    const cv::UMat& matchingMask() const { return matching_mask_; }
    void setMatchingMask(const cv::UMat &mask)
    {
        CV_Assert(mask.type() == CV_8U && mask.cols == mask.rows);
        matching_mask_ = mask.clone();
    }

    Ptr<detail::BundleAdjusterBase> bundleAdjuster() { return bundle_adjuster_; }
    const Ptr<detail::BundleAdjusterBase> bundleAdjuster() const { return bundle_adjuster_; }
    void setBundleAdjuster(Ptr<detail::BundleAdjusterBase> bundle_adjuster)
        { bundle_adjuster_ = bundle_adjuster; }

    Ptr<detail::Estimator> estimator() { return estimator_; }
    const Ptr<detail::Estimator> estimator() const { return estimator_; }
    void setEstimator(Ptr<detail::Estimator> estimator)
        { estimator_ = estimator; }

    Ptr<WarperCreator> warper() { return warper_; }
    const Ptr<WarperCreator> warper() const { return warper_; }
    void setWarper(Ptr<WarperCreator> creator) { warper_ = creator; }

    Ptr<detail::ExposureCompensator> exposureCompensator() { return exposure_comp_; }
    const Ptr<detail::ExposureCompensator> exposureCompensator() const { return exposure_comp_; }
    void setExposureCompensator(Ptr<detail::ExposureCompensator> exposure_comp)
        { exposure_comp_ = exposure_comp; }

    Ptr<detail::SeamFinder> seamFinder() { return seam_finder_; }
    const Ptr<detail::SeamFinder> seamFinder() const { return seam_finder_; }
    void setSeamFinder(Ptr<detail::SeamFinder> seam_finder) { seam_finder_ = seam_finder; }

    Ptr<detail::Blender> blender() { return blender_; }
    const Ptr<detail::Blender> blender() const { return blender_; }
    void setBlender(Ptr<detail::Blender> b) { blender_ = b; }


    CV_WRAP Status estimateTransform(InputArrayOfArrays images, InputArrayOfArrays masks = noArray());


    CV_WRAP Status composePanorama(OutputArray pano);

    Status composePanorama(InputArrayOfArrays images, OutputArray pano);


    CV_WRAP Status stitch(InputArrayOfArrays images, OutputArray pano);

    CV_WRAP Status stitch(InputArrayOfArrays images, InputArrayOfArrays masks, OutputArray pano);

    std::vector<int> component() const { return indices_; }
    std::vector<detail::CameraParams> cameras() const { return cameras_; }
    CV_WRAP double workScale() const { return work_scale_; }
    UMat resultMask() const { return result_mask_; }

private:
    Status matchImages();
    Status estimateCameraParams();

    double registr_resol_;
    double seam_est_resol_;
    double compose_resol_;
    double conf_thresh_;
    InterpolationFlags interp_flags_;
    Ptr<Feature2D> features_finder_;
    Ptr<detail::FeaturesMatcher> features_matcher_;
    cv::UMat matching_mask_;
    Ptr<detail::BundleAdjusterBase> bundle_adjuster_;
    Ptr<detail::Estimator> estimator_;
    bool do_wave_correct_;
    detail::WaveCorrectKind wave_correct_kind_;
    Ptr<WarperCreator> warper_;
    Ptr<detail::ExposureCompensator> exposure_comp_;
    Ptr<detail::SeamFinder> seam_finder_;
    Ptr<detail::Blender> blender_;

    std::vector<cv::UMat> imgs_;
    std::vector<cv::UMat> masks_;
    std::vector<cv::Size> full_img_sizes_;
    std::vector<detail::ImageFeatures> features_;
    std::vector<detail::MatchesInfo> pairwise_matches_;
    std::vector<cv::UMat> seam_est_imgs_;
    std::vector<int> indices_;
    std::vector<detail::CameraParams> cameras_;
    UMat result_mask_;
    double work_scale_;
    double seam_scale_;
    double seam_work_aspect_;
    double warped_image_scale_;
};


CV_DEPRECATED Ptr<Stitcher> createStitcher(bool try_use_gpu = false);


CV_DEPRECATED Ptr<Stitcher> createStitcherScans(bool try_use_gpu = false);



}

#endif
