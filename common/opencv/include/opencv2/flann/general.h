

#ifndef OPENCV_FLANN_GENERAL_H_
#define OPENCV_FLANN_GENERAL_H_

#include "opencv2/core.hpp"

namespace cvflann
{

class FLANNException : public cv::Exception
{
public:
    FLANNException(const char* message) : cv::Exception(0, message, "", __FILE__, __LINE__) { }

    FLANNException(const cv::String& message) : cv::Exception(0, message, "", __FILE__, __LINE__) { }
};

}


#endif
