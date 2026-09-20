






#ifndef OPENCV_GAPI_OWN_TYPES_HPP
#define OPENCV_GAPI_OWN_TYPES_HPP

#   if defined(__OPENCV_BUILD)
#       include <opencv2/core/base.hpp>
#       define GAPI_EXPORTS CV_EXPORTS
#   else
#       define GAPI_EXPORTS

#if 0
#       if defined _WIN32
#           define GAPI_EXPORTS __declspec(dllexport)
#       elif defined __GNUC__ && __GNUC__ >= 4
#           define GAPI_EXPORTS __attribute__ ((visibility ("default")))
#       endif

#       ifndef GAPI_EXPORTS
#           define GAPI_EXPORTS
#       endif
#endif

#   endif

#endif
