

#ifndef OPENCV_STITCHING_CAMERA_HPP
#define OPENCV_STITCHING_CAMERA_HPP

#include "opencv2/core.hpp"

namespace cv {
namespace detail {





struct CV_EXPORTS_W_SIMPLE CameraParams
{
    CameraParams();
    CameraParams(const CameraParams& other);
    CameraParams& operator =(const CameraParams& other);
    CV_WRAP Mat K() const;

    CV_PROP_RW double focal;
    CV_PROP_RW double aspect;
    CV_PROP_RW double ppx;
    CV_PROP_RW double ppy;
    CV_PROP_RW Mat R;
    CV_PROP_RW Mat t;
};



}
}

#endif
