



#ifndef OPENCV_DNN_LAYER_DETAILS_HPP
#define OPENCV_DNN_LAYER_DETAILS_HPP

#include <opencv2/dnn/layer.hpp>

namespace cv {
namespace dnn {
CV__DNN_INLINE_NS_BEGIN


#define CV_DNN_REGISTER_LAYER_FUNC(type, constructorFunc) \
    cv::dnn::LayerFactory::registerLayer(#type, constructorFunc);


#define CV_DNN_REGISTER_LAYER_CLASS(type, class) \
    cv::dnn::LayerFactory::registerLayer(#type, cv::dnn::details::_layerDynamicRegisterer<class>);


#define CV_DNN_REGISTER_LAYER_FUNC_STATIC(type, constructorFunc) \
static cv::dnn::details::_LayerStaticRegisterer __LayerStaticRegisterer_##type(#type, constructorFunc);


#define CV_DNN_REGISTER_LAYER_CLASS_STATIC(type, class)                         \
Ptr<Layer> __LayerStaticRegisterer_func_##type(LayerParams &params) \
    { return Ptr<Layer>(new class(params)); }                       \
static cv::dnn::details::_LayerStaticRegisterer __LayerStaticRegisterer_##type(#type, __LayerStaticRegisterer_func_##type);

namespace details {

template<typename LayerClass>
Ptr<Layer> _layerDynamicRegisterer(LayerParams &params)
{
    return Ptr<Layer>(LayerClass::create(params));
}


class _LayerStaticRegisterer
{
    String type;
public:

    _LayerStaticRegisterer(const String &layerType, LayerFactory::Constructor layerConstructor)
    {
        this->type = layerType;
        LayerFactory::registerLayer(layerType, layerConstructor);
    }

    ~_LayerStaticRegisterer()
    {
        LayerFactory::unregisterLayer(type);
    }
};

}
CV__DNN_INLINE_NS_END
}}

#endif
