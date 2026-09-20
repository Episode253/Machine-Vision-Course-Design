

#ifndef OPENCV_STITCHING_AUTOCALIB_HPP
#define OPENCV_STITCHING_AUTOCALIB_HPP

#include "opencv2/core.hpp"
#include "matchers.hpp"

namespace cv {
namespace detail {





void CV_EXPORTS_W focalsFromHomography(const Mat &H, double &f0, double &f1, bool &f0_ok, bool &f1_ok);


void CV_EXPORTS estimateFocal(const std::vector<ImageFeatures> &features,
                              const std::vector<MatchesInfo> &pairwise_matches,
                              std::vector<double> &focals);

bool CV_EXPORTS_W calibrateRotatingCamera(const std::vector<Mat> &Hs,CV_OUT Mat &K);



}
}

#endif
