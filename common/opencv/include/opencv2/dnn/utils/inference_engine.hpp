






#ifndef OPENCV_DNN_UTILS_INF_ENGINE_HPP
#define OPENCV_DNN_UTILS_INF_ENGINE_HPP

#include "../dnn.hpp"

namespace cv { namespace dnn {
CV__DNN_INLINE_NS_BEGIN



CV_EXPORTS_W void resetMyriadDevice();



#define CV_DNN_INFERENCE_ENGINE_VPU_TYPE_UNSPECIFIED ""

#define CV_DNN_INFERENCE_ENGINE_VPU_TYPE_MYRIAD_2 "Myriad2"

#define CV_DNN_INFERENCE_ENGINE_VPU_TYPE_MYRIAD_X "MyriadX"



CV_EXPORTS_W cv::String getInferenceEngineVPUType();


CV__DNN_INLINE_NS_END
}}

#endif
