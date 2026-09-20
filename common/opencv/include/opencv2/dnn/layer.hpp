

#ifndef OPENCV_DNN_LAYER_HPP
#define OPENCV_DNN_LAYER_HPP
#include <opencv2/dnn.hpp>

namespace cv {
namespace dnn {
CV__DNN_INLINE_NS_BEGIN







class CV_EXPORTS LayerFactory
{
public:


    typedef Ptr<Layer>(*Constructor)(LayerParams &params);


    static void registerLayer(const String &type, Constructor constructor);


    static void unregisterLayer(const String &type);


    static Ptr<Layer> createLayerInstance(const String &type, LayerParams& params);

private:
    LayerFactory();
};



CV__DNN_INLINE_NS_END
}
}
#endif
