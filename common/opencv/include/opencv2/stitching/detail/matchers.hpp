

#ifndef OPENCV_STITCHING_MATCHERS_HPP
#define OPENCV_STITCHING_MATCHERS_HPP

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"

#include "opencv2/opencv_modules.hpp"

namespace cv {
namespace detail {





struct CV_EXPORTS_W_SIMPLE ImageFeatures
{
    CV_PROP_RW int img_idx;
    CV_PROP_RW Size img_size;
    std::vector<KeyPoint> keypoints;
    CV_PROP_RW UMat descriptors;
    CV_WRAP std::vector<KeyPoint> getKeypoints() { return keypoints; };
};

CV_EXPORTS_W void computeImageFeatures(
    const Ptr<Feature2D> &featuresFinder,
    InputArrayOfArrays  images,
    CV_OUT std::vector<ImageFeatures> &features,
    InputArrayOfArrays masks = noArray());


CV_EXPORTS_AS(computeImageFeatures2) void computeImageFeatures(
    const Ptr<Feature2D> &featuresFinder,
    InputArray image,
    CV_OUT ImageFeatures &features,
    InputArray mask = noArray());


struct CV_EXPORTS_W_SIMPLE MatchesInfo
{
    MatchesInfo();
    MatchesInfo(const MatchesInfo &other);
    MatchesInfo& operator =(const MatchesInfo &other);

    CV_PROP_RW int src_img_idx;
    CV_PROP_RW int dst_img_idx;
    std::vector<DMatch> matches;
    std::vector<uchar> inliers_mask;
    CV_PROP_RW int num_inliers;
    CV_PROP_RW Mat H;
    CV_PROP_RW double confidence;
    CV_WRAP std::vector<DMatch> getMatches() { return matches; };
    CV_WRAP std::vector<uchar> getInliers() { return inliers_mask; };
};


class CV_EXPORTS_W FeaturesMatcher
{
public:
    CV_WRAP virtual ~FeaturesMatcher() {}


    CV_WRAP_AS(apply) void operator ()(const ImageFeatures &features1, const ImageFeatures &features2,
                     CV_OUT MatchesInfo& matches_info) { match(features1, features2, matches_info); }


    CV_WRAP_AS(apply2) void operator ()(const std::vector<ImageFeatures> &features, CV_OUT std::vector<MatchesInfo> &pairwise_matches,
                     const cv::UMat &mask = cv::UMat());


   CV_WRAP bool isThreadSafe() const { return is_thread_safe_; }


   CV_WRAP virtual void collectGarbage() {}

protected:
    FeaturesMatcher(bool is_thread_safe = false) : is_thread_safe_(is_thread_safe) {}


    virtual void match(const ImageFeatures &features1, const ImageFeatures &features2,
                       MatchesInfo& matches_info) = 0;

    bool is_thread_safe_;
};


class CV_EXPORTS_W BestOf2NearestMatcher : public FeaturesMatcher
{
public:

    CV_WRAP BestOf2NearestMatcher(bool try_use_gpu = false, float match_conf = 0.3f, int num_matches_thresh1 = 6,
                          int num_matches_thresh2 = 6);

    CV_WRAP void collectGarbage() CV_OVERRIDE;
    CV_WRAP static Ptr<BestOf2NearestMatcher> create(bool try_use_gpu = false, float match_conf = 0.3f, int num_matches_thresh1 = 6,
        int num_matches_thresh2 = 6);

protected:

    void match(const ImageFeatures &features1, const ImageFeatures &features2, MatchesInfo &matches_info) CV_OVERRIDE;
    int num_matches_thresh1_;
    int num_matches_thresh2_;
    Ptr<FeaturesMatcher> impl_;
};

class CV_EXPORTS_W BestOf2NearestRangeMatcher : public BestOf2NearestMatcher
{
public:
    CV_WRAP BestOf2NearestRangeMatcher(int range_width = 5, bool try_use_gpu = false, float match_conf = 0.3f,
                            int num_matches_thresh1 = 6, int num_matches_thresh2 = 6);

    void operator ()(const std::vector<ImageFeatures> &features, std::vector<MatchesInfo> &pairwise_matches,
                     const cv::UMat &mask = cv::UMat());


protected:
    int range_width_;
};


class CV_EXPORTS_W AffineBestOf2NearestMatcher : public BestOf2NearestMatcher
{
public:

    CV_WRAP AffineBestOf2NearestMatcher(bool full_affine = false, bool try_use_gpu = false,
                                float match_conf = 0.3f, int num_matches_thresh1 = 6) :
        BestOf2NearestMatcher(try_use_gpu, match_conf, num_matches_thresh1, num_matches_thresh1),
        full_affine_(full_affine) {}

protected:
    void match(const ImageFeatures &features1, const ImageFeatures &features2, MatchesInfo &matches_info) CV_OVERRIDE;

    bool full_affine_;
};



}
}

#endif
