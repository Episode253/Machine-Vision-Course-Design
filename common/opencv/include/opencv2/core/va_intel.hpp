






#ifndef OPENCV_CORE_VA_INTEL_HPP
#define OPENCV_CORE_VA_INTEL_HPP

#ifndef __cplusplus
#  error va_intel.hpp header must be compiled as C++
#endif

#include "opencv2/core.hpp"
#include "ocl.hpp"

#if defined(HAVE_VA)
# include "va/va.h"
#else
# if !defined(_VA_H_)
    typedef void* VADisplay;
    typedef unsigned int VASurfaceID;
# endif
#endif

namespace cv { namespace va_intel {






namespace ocl {
using namespace cv::ocl;



CV_EXPORTS Context& initializeContextFromVA(VADisplay display, bool tryInterop = true);

}


CV_EXPORTS void convertToVASurface(VADisplay display, InputArray src, VASurfaceID surface, Size size);


CV_EXPORTS void convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst);



}}

#endif
