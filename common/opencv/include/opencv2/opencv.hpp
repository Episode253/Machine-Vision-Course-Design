

#ifndef OPENCV_ALL_HPP
#define OPENCV_ALL_HPP



#include "opencv2/opencv_modules.hpp"



#include "opencv2/core.hpp"


#ifdef HAVE_OPENCV_CALIB3D
#include "opencv2/calib3d.hpp"
#endif
#ifdef HAVE_OPENCV_FEATURES2D
#include "opencv2/features2d.hpp"
#endif
#ifdef HAVE_OPENCV_DNN
#include "opencv2/dnn.hpp"
#endif
#ifdef HAVE_OPENCV_FLANN
#include "opencv2/flann.hpp"
#endif
#ifdef HAVE_OPENCV_HIGHGUI
#include "opencv2/highgui.hpp"
#endif
#ifdef HAVE_OPENCV_IMGCODECS
#include "opencv2/imgcodecs.hpp"
#endif
#ifdef HAVE_OPENCV_IMGPROC
#include "opencv2/imgproc.hpp"
#endif
#ifdef HAVE_OPENCV_ML
#include "opencv2/ml.hpp"
#endif
#ifdef HAVE_OPENCV_OBJDETECT
#include "opencv2/objdetect.hpp"
#endif
#ifdef HAVE_OPENCV_PHOTO
#include "opencv2/photo.hpp"
#endif
#ifdef HAVE_OPENCV_SHAPE
#include "opencv2/shape.hpp"
#endif
#ifdef HAVE_OPENCV_STITCHING
#include "opencv2/stitching.hpp"
#endif
#ifdef HAVE_OPENCV_SUPERRES
#include "opencv2/superres.hpp"
#endif
#ifdef HAVE_OPENCV_VIDEO
#include "opencv2/video.hpp"
#endif
#ifdef HAVE_OPENCV_VIDEOIO
#include "opencv2/videoio.hpp"
#endif
#ifdef HAVE_OPENCV_VIDEOSTAB
#include "opencv2/videostab.hpp"
#endif
#ifdef HAVE_OPENCV_VIZ
#include "opencv2/viz.hpp"
#endif


#ifdef HAVE_OPENCV_CUDAARITHM
#include "opencv2/cudaarithm.hpp"
#endif
#ifdef HAVE_OPENCV_CUDABGSEGM
#include "opencv2/cudabgsegm.hpp"
#endif
#ifdef HAVE_OPENCV_CUDACODEC
#include "opencv2/cudacodec.hpp"
#endif
#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include "opencv2/cudafeatures2d.hpp"
#endif
#ifdef HAVE_OPENCV_CUDAFILTERS
#include "opencv2/cudafilters.hpp"
#endif
#ifdef HAVE_OPENCV_CUDAIMGPROC
#include "opencv2/cudaimgproc.hpp"
#endif
#ifdef HAVE_OPENCV_CUDAOBJDETECT
#include "opencv2/cudaobjdetect.hpp"
#endif
#ifdef HAVE_OPENCV_CUDAOPTFLOW
#include "opencv2/cudaoptflow.hpp"
#endif
#ifdef HAVE_OPENCV_CUDASTEREO
#include "opencv2/cudastereo.hpp"
#endif
#ifdef HAVE_OPENCV_CUDAWARPING
#include "opencv2/cudawarping.hpp"
#endif

#endif
