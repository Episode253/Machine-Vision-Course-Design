

#ifndef OPENCV_STITCHING_MOTION_ESTIMATORS_HPP
#define OPENCV_STITCHING_MOTION_ESTIMATORS_HPP

#include "opencv2/core.hpp"
#include "matchers.hpp"
#include "util.hpp"
#include "camera.hpp"

namespace cv {
namespace detail {





class CV_EXPORTS_W Estimator
{
public:
    virtual ~Estimator() {}


    CV_WRAP_AS(apply) bool operator ()(const std::vector<ImageFeatures> &features,
        const std::vector<MatchesInfo> &pairwise_matches,
        CV_OUT CV_IN_OUT std::vector<CameraParams> &cameras)
    {
        return estimate(features, pairwise_matches, cameras);
    }

protected:

    virtual bool estimate(const std::vector<ImageFeatures> &features,
                          const std::vector<MatchesInfo> &pairwise_matches,
                          CV_OUT std::vector<CameraParams> &cameras) = 0;
};


class CV_EXPORTS_W HomographyBasedEstimator : public Estimator
{
public:
    CV_WRAP HomographyBasedEstimator(bool is_focals_estimated = false)
        : is_focals_estimated_(is_focals_estimated) {}

private:
    virtual bool estimate(const std::vector<ImageFeatures> &features,
                          const std::vector<MatchesInfo> &pairwise_matches,
                          std::vector<CameraParams> &cameras) CV_OVERRIDE;

    bool is_focals_estimated_;
};


class CV_EXPORTS_W AffineBasedEstimator : public Estimator
{
public:
    CV_WRAP AffineBasedEstimator(){}
private:
    virtual bool estimate(const std::vector<ImageFeatures> &features,
                          const std::vector<MatchesInfo> &pairwise_matches,
                          std::vector<CameraParams> &cameras) CV_OVERRIDE;
};


class CV_EXPORTS_W BundleAdjusterBase : public Estimator
{
public:
    CV_WRAP const Mat refinementMask() const { return refinement_mask_.clone(); }
    CV_WRAP void setRefinementMask(const Mat &mask)
    {
        CV_Assert(mask.type() == CV_8U && mask.size() == Size(3, 3));
        refinement_mask_ = mask.clone();
    }

    CV_WRAP double confThresh() const { return conf_thresh_; }
    CV_WRAP void setConfThresh(double conf_thresh) { conf_thresh_ = conf_thresh; }

    CV_WRAP TermCriteria termCriteria() { return term_criteria_; }
    CV_WRAP void setTermCriteria(const TermCriteria& term_criteria) { term_criteria_ = term_criteria; }

protected:

    BundleAdjusterBase(int num_params_per_cam, int num_errs_per_measurement)
        : num_images_(0), total_num_matches_(0),
          num_params_per_cam_(num_params_per_cam),
          num_errs_per_measurement_(num_errs_per_measurement),
          features_(0), pairwise_matches_(0), conf_thresh_(0)
    {
        setRefinementMask(Mat::ones(3, 3, CV_8U));
        setConfThresh(1.);
        setTermCriteria(TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 1000, DBL_EPSILON));
    }


    virtual bool estimate(const std::vector<ImageFeatures> &features,
                          const std::vector<MatchesInfo> &pairwise_matches,
                          std::vector<CameraParams> &cameras) CV_OVERRIDE;


    virtual void setUpInitialCameraParams(const std::vector<CameraParams> &cameras) = 0;

    virtual void obtainRefinedCameraParams(std::vector<CameraParams> &cameras) const = 0;

    virtual void calcError(Mat &err) = 0;

    virtual void calcJacobian(Mat &jac) = 0;


    Mat refinement_mask_;

    int num_images_;
    int total_num_matches_;

    int num_params_per_cam_;
    int num_errs_per_measurement_;

    const ImageFeatures *features_;
    const MatchesInfo *pairwise_matches_;


    double conf_thresh_;


    TermCriteria term_criteria_;


    Mat cam_params_;


    std::vector<std::pair<int,int> > edges_;
};



class CV_EXPORTS_W NoBundleAdjuster : public BundleAdjusterBase
{
public:
    CV_WRAP NoBundleAdjuster() : BundleAdjusterBase(0, 0) {}

private:
    bool estimate(const std::vector<ImageFeatures> &, const std::vector<MatchesInfo> &,
                  std::vector<CameraParams> &) CV_OVERRIDE
    {
        return true;
    }
    void setUpInitialCameraParams(const std::vector<CameraParams> &) CV_OVERRIDE {}
    void obtainRefinedCameraParams(std::vector<CameraParams> &) const CV_OVERRIDE {}
    void calcError(Mat &) CV_OVERRIDE {}
    void calcJacobian(Mat &) CV_OVERRIDE {}
};



class CV_EXPORTS_W BundleAdjusterReproj : public BundleAdjusterBase
{
public:
    CV_WRAP BundleAdjusterReproj() : BundleAdjusterBase(7, 2) {}

private:
    void setUpInitialCameraParams(const std::vector<CameraParams> &cameras) CV_OVERRIDE;
    void obtainRefinedCameraParams(std::vector<CameraParams> &cameras) const CV_OVERRIDE;
    void calcError(Mat &err) CV_OVERRIDE;
    void calcJacobian(Mat &jac) CV_OVERRIDE;

    Mat err1_, err2_;
};



class CV_EXPORTS_W BundleAdjusterRay : public BundleAdjusterBase
{
public:
    CV_WRAP BundleAdjusterRay() : BundleAdjusterBase(4, 3) {}

private:
    void setUpInitialCameraParams(const std::vector<CameraParams> &cameras) CV_OVERRIDE;
    void obtainRefinedCameraParams(std::vector<CameraParams> &cameras) const CV_OVERRIDE;
    void calcError(Mat &err) CV_OVERRIDE;
    void calcJacobian(Mat &jac) CV_OVERRIDE;

    Mat err1_, err2_;
};



class CV_EXPORTS_W BundleAdjusterAffine : public BundleAdjusterBase
{
public:
    CV_WRAP BundleAdjusterAffine() : BundleAdjusterBase(6, 2) {}

private:
    void setUpInitialCameraParams(const std::vector<CameraParams> &cameras) CV_OVERRIDE;
    void obtainRefinedCameraParams(std::vector<CameraParams> &cameras) const CV_OVERRIDE;
    void calcError(Mat &err) CV_OVERRIDE;
    void calcJacobian(Mat &jac) CV_OVERRIDE;

    Mat err1_, err2_;
};



class CV_EXPORTS_W BundleAdjusterAffinePartial : public BundleAdjusterBase
{
public:
    CV_WRAP BundleAdjusterAffinePartial() : BundleAdjusterBase(4, 2) {}

private:
    void setUpInitialCameraParams(const std::vector<CameraParams> &cameras) CV_OVERRIDE;
    void obtainRefinedCameraParams(std::vector<CameraParams> &cameras) const CV_OVERRIDE;
    void calcError(Mat &err) CV_OVERRIDE;
    void calcJacobian(Mat &jac) CV_OVERRIDE;

    Mat err1_, err2_;
};


enum WaveCorrectKind
{
    WAVE_CORRECT_HORIZ,
    WAVE_CORRECT_VERT
};


void CV_EXPORTS_W waveCorrect(CV_IN_OUT std::vector<Mat> &rmats, WaveCorrectKind kind);






String CV_EXPORTS_W matchesGraphAsString(std::vector<String> &pathes, std::vector<MatchesInfo> &pairwise_matches,
                                            float conf_threshold);

CV_EXPORTS_W std::vector<int>  leaveBiggestComponent(
        std::vector<ImageFeatures> &features,
        std::vector<MatchesInfo> &pairwise_matches,
        float conf_threshold);

void CV_EXPORTS findMaxSpanningTree(
        int num_images, const std::vector<MatchesInfo> &pairwise_matches,
        Graph &span_tree, std::vector<int> &centers);



}
}

#endif
