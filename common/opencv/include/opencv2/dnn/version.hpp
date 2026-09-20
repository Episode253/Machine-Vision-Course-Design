



#ifndef OPENCV_DNN_VERSION_HPP
#define OPENCV_DNN_VERSION_HPP


#define OPENCV_DNN_API_VERSION 20190621

#if !defined CV_DOXYGEN && !defined CV_STATIC_ANALYSIS && !defined CV_DNN_DONT_ADD_INLINE_NS
#define CV__DNN_INLINE_NS __CV_CAT(dnn4_v, OPENCV_DNN_API_VERSION)
#define CV__DNN_INLINE_NS_BEGIN namespace CV__DNN_INLINE_NS {
#define CV__DNN_INLINE_NS_END }
namespace cv { namespace dnn { namespace CV__DNN_INLINE_NS { } using namespace CV__DNN_INLINE_NS; }}
#else
#define CV__DNN_INLINE_NS_BEGIN
#define CV__DNN_INLINE_NS_END
#endif

#endif
