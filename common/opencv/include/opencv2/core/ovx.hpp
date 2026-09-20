








#pragma once
#ifndef OPENCV_OVX_HPP
#define OPENCV_OVX_HPP

#include "cvdef.h"

namespace cv
{

CV_EXPORTS_W bool haveOpenVX();


CV_EXPORTS_W bool useOpenVX();


CV_EXPORTS_W void setUseOpenVX(bool flag);
}

#endif
