

#ifndef OPENCV_DNN_DNN_HPP
#define OPENCV_DNN_DNN_HPP

#include <vector>
#include <opencv2/core.hpp>
#include "opencv2/core/async.hpp"

#include "../dnn/version.hpp"

#include <opencv2/dnn/dict.hpp>

namespace cv {
namespace dnn {
CV__DNN_INLINE_NS_BEGIN



    typedef std::vector<int> MatShape;


    enum Backend
    {



        DNN_BACKEND_DEFAULT,
        DNN_BACKEND_HALIDE,
        DNN_BACKEND_INFERENCE_ENGINE,
        DNN_BACKEND_OPENCV,
        DNN_BACKEND_VKCOM
    };


    enum Target
    {
        DNN_TARGET_CPU,
        DNN_TARGET_OPENCL,
        DNN_TARGET_OPENCL_FP16,
        DNN_TARGET_MYRIAD,
        DNN_TARGET_VULKAN,
        DNN_TARGET_FPGA
    };

    CV_EXPORTS std::vector< std::pair<Backend, Target> > getAvailableBackends();
    CV_EXPORTS std::vector<Target> getAvailableTargets(Backend be);


    class CV_EXPORTS LayerParams : public Dict
    {
    public:

        std::vector<Mat> blobs;

        String name;
        String type;
    };


    class BackendNode
    {
    public:
        BackendNode(int backendId);

        virtual ~BackendNode();

        int backendId;
    };


    class BackendWrapper
    {
    public:
        BackendWrapper(int backendId, int targetId);


        BackendWrapper(int targetId, const cv::Mat& m);


        BackendWrapper(const Ptr<BackendWrapper>& base, const MatShape& shape);

        virtual ~BackendWrapper();


        virtual void copyToHost() = 0;


        virtual void setHostDirty() = 0;

        int backendId;
        int targetId;
    };

    class CV_EXPORTS ActivationLayer;


    class CV_EXPORTS_W Layer : public Algorithm
    {
    public:


        CV_PROP_RW std::vector<Mat> blobs;


        CV_DEPRECATED_EXTERNAL
        virtual void finalize(const std::vector<Mat*> &input, std::vector<Mat> &output);


        CV_WRAP virtual void finalize(InputArrayOfArrays inputs, OutputArrayOfArrays outputs);


        CV_DEPRECATED_EXTERNAL
        virtual void forward(std::vector<Mat*> &input, std::vector<Mat> &output, std::vector<Mat> &internals);


        virtual void forward(InputArrayOfArrays inputs, OutputArrayOfArrays outputs, OutputArrayOfArrays internals);


        void forward_fallback(InputArrayOfArrays inputs, OutputArrayOfArrays outputs, OutputArrayOfArrays internals);


        CV_DEPRECATED_EXTERNAL
        void finalize(const std::vector<Mat> &inputs, CV_OUT std::vector<Mat> &outputs);


        CV_DEPRECATED std::vector<Mat> finalize(const std::vector<Mat> &inputs);


        CV_DEPRECATED CV_WRAP void run(const std::vector<Mat> &inputs, CV_OUT std::vector<Mat> &outputs,
                                       CV_IN_OUT std::vector<Mat> &internals);


        virtual int inputNameToIndex(String inputName);

        CV_WRAP virtual int outputNameToIndex(const String& outputName);


        virtual bool supportBackend(int backendId);


        virtual Ptr<BackendNode> initHalide(const std::vector<Ptr<BackendWrapper> > &inputs);

        virtual Ptr<BackendNode> initInfEngine(const std::vector<Ptr<BackendWrapper> > &inputs);

        virtual Ptr<BackendNode> initVkCom(const std::vector<Ptr<BackendWrapper> > &inputs);

        virtual void applyHalideScheduler(Ptr<BackendNode>& node,
                                          const std::vector<Mat*> &inputs,
                                          const std::vector<Mat> &outputs,
                                          int targetId) const;


        virtual Ptr<BackendNode> tryAttach(const Ptr<BackendNode>& node);


        virtual bool setActivation(const Ptr<ActivationLayer>& layer);


        virtual bool tryFuse(Ptr<Layer>& top);


        virtual void getScaleShift(Mat& scale, Mat& shift) const;


        virtual void unsetAttached();

        virtual bool getMemoryShapes(const std::vector<MatShape> &inputs,
                                     const int requiredOutputs,
                                     std::vector<MatShape> &outputs,
                                     std::vector<MatShape> &internals) const;
        virtual int64 getFLOPS(const std::vector<MatShape> &inputs,
                               const std::vector<MatShape> &outputs) const {CV_UNUSED(inputs); CV_UNUSED(outputs); return 0;}

        CV_PROP String name;
        CV_PROP String type;
        CV_PROP int preferableTarget;

        Layer();
        explicit Layer(const LayerParams &params);
        void setParamsFrom(const LayerParams &params);
        virtual ~Layer();
    };


    class CV_EXPORTS_W_SIMPLE Net
    {
    public:

        CV_WRAP Net();
        CV_WRAP ~Net();


        CV_WRAP static Net readFromModelOptimizer(const String& xml, const String& bin);


        CV_WRAP bool empty() const;


        CV_WRAP String dump();

        CV_WRAP void dumpToFile(const String& path);

        int addLayer(const String &name, const String &type, LayerParams &params);

        int addLayerToPrev(const String &name, const String &type, LayerParams &params);


        CV_WRAP int getLayerId(const String &layer);

        CV_WRAP std::vector<String> getLayerNames() const;


        typedef DictValue LayerId;


        CV_WRAP Ptr<Layer> getLayer(LayerId layerId);


        std::vector<Ptr<Layer> > getLayerInputs(LayerId layerId);


        CV_WRAP void connect(String outPin, String inpPin);


        void connect(int outLayerId, int outNum, int inpLayerId, int inpNum);


        CV_WRAP void setInputsNames(const std::vector<String> &inputBlobNames);


        CV_WRAP Mat forward(const String& outputName = String());


        CV_WRAP AsyncArray forwardAsync(const String& outputName = String());


        CV_WRAP void forward(OutputArrayOfArrays outputBlobs, const String& outputName = String());


        CV_WRAP void forward(OutputArrayOfArrays outputBlobs,
                             const std::vector<String>& outBlobNames);


        CV_WRAP_AS(forwardAndRetrieve) void forward(CV_OUT std::vector<std::vector<Mat> >& outputBlobs,
                                                    const std::vector<String>& outBlobNames);


        CV_WRAP void setHalideScheduler(const String& scheduler);


        CV_WRAP void setPreferableBackend(int backendId);


        CV_WRAP void setPreferableTarget(int targetId);


        CV_WRAP void setInput(InputArray blob, const String& name = "",
                              double scalefactor = 1.0, const Scalar& mean = Scalar());


        CV_WRAP void setParam(LayerId layer, int numParam, const Mat &blob);


        CV_WRAP Mat getParam(LayerId layer, int numParam = 0);


        CV_WRAP std::vector<int> getUnconnectedOutLayers() const;


        CV_WRAP std::vector<String> getUnconnectedOutLayersNames() const;


        CV_WRAP void getLayersShapes(const std::vector<MatShape>& netInputShapes,
                                     CV_OUT std::vector<int>& layersIds,
                                     CV_OUT std::vector<std::vector<MatShape> >& inLayersShapes,
                                     CV_OUT std::vector<std::vector<MatShape> >& outLayersShapes) const;


        CV_WRAP void getLayersShapes(const MatShape& netInputShape,
                                     CV_OUT std::vector<int>& layersIds,
                                     CV_OUT std::vector<std::vector<MatShape> >& inLayersShapes,
                                     CV_OUT std::vector<std::vector<MatShape> >& outLayersShapes) const;


        void getLayerShapes(const MatShape& netInputShape,
                                    const int layerId,
                                    CV_OUT std::vector<MatShape>& inLayerShapes,
                                    CV_OUT std::vector<MatShape>& outLayerShapes) const;


        void getLayerShapes(const std::vector<MatShape>& netInputShapes,
                                    const int layerId,
                                    CV_OUT std::vector<MatShape>& inLayerShapes,
                                    CV_OUT std::vector<MatShape>& outLayerShapes) const;


        CV_WRAP int64 getFLOPS(const std::vector<MatShape>& netInputShapes) const;

        CV_WRAP int64 getFLOPS(const MatShape& netInputShape) const;

        CV_WRAP int64 getFLOPS(const int layerId,
                               const std::vector<MatShape>& netInputShapes) const;

        CV_WRAP int64 getFLOPS(const int layerId,
                               const MatShape& netInputShape) const;


        CV_WRAP void getLayerTypes(CV_OUT std::vector<String>& layersTypes) const;


        CV_WRAP int getLayersCount(const String& layerType) const;


        void getMemoryConsumption(const std::vector<MatShape>& netInputShapes,
                                          CV_OUT size_t& weights, CV_OUT size_t& blobs) const;

        CV_WRAP void getMemoryConsumption(const MatShape& netInputShape,
                                          CV_OUT size_t& weights, CV_OUT size_t& blobs) const;

        CV_WRAP void getMemoryConsumption(const int layerId,
                                          const std::vector<MatShape>& netInputShapes,
                                          CV_OUT size_t& weights, CV_OUT size_t& blobs) const;

        CV_WRAP void getMemoryConsumption(const int layerId,
                                          const MatShape& netInputShape,
                                          CV_OUT size_t& weights, CV_OUT size_t& blobs) const;


        void getMemoryConsumption(const std::vector<MatShape>& netInputShapes,
                                          CV_OUT std::vector<int>& layerIds,
                                          CV_OUT std::vector<size_t>& weights,
                                          CV_OUT std::vector<size_t>& blobs) const;

        void getMemoryConsumption(const MatShape& netInputShape,
                                          CV_OUT std::vector<int>& layerIds,
                                          CV_OUT std::vector<size_t>& weights,
                                          CV_OUT std::vector<size_t>& blobs) const;


        CV_WRAP void enableFusion(bool fusion);


        CV_WRAP int64 getPerfProfile(CV_OUT std::vector<double>& timings);

    private:
        struct Impl;
        Ptr<Impl> impl;
    };


    CV_EXPORTS_W Net readNetFromDarknet(const String &cfgFile, const String &darknetModel = String());


    CV_EXPORTS_W Net readNetFromDarknet(const std::vector<uchar>& bufferCfg,
                                        const std::vector<uchar>& bufferModel = std::vector<uchar>());


    CV_EXPORTS Net readNetFromDarknet(const char *bufferCfg, size_t lenCfg,
                                      const char *bufferModel = NULL, size_t lenModel = 0);


    CV_EXPORTS_W Net readNetFromCaffe(const String &prototxt, const String &caffeModel = String());


    CV_EXPORTS_W Net readNetFromCaffe(const std::vector<uchar>& bufferProto,
                                      const std::vector<uchar>& bufferModel = std::vector<uchar>());


    CV_EXPORTS Net readNetFromCaffe(const char *bufferProto, size_t lenProto,
                                    const char *bufferModel = NULL, size_t lenModel = 0);


    CV_EXPORTS_W Net readNetFromTensorflow(const String &model, const String &config = String());


    CV_EXPORTS_W Net readNetFromTensorflow(const std::vector<uchar>& bufferModel,
                                           const std::vector<uchar>& bufferConfig = std::vector<uchar>());


    CV_EXPORTS Net readNetFromTensorflow(const char *bufferModel, size_t lenModel,
                                         const char *bufferConfig = NULL, size_t lenConfig = 0);


     CV_EXPORTS_W Net readNetFromTorch(const String &model, bool isBinary = true, bool evaluate = true);


     CV_EXPORTS_W Net readNet(const String& model, const String& config = "", const String& framework = "");


     CV_EXPORTS_W Net readNet(const String& framework, const std::vector<uchar>& bufferModel,
                              const std::vector<uchar>& bufferConfig = std::vector<uchar>());


    CV_EXPORTS_W Mat readTorchBlob(const String &filename, bool isBinary = true);


    CV_EXPORTS_W Net readNetFromModelOptimizer(const String &xml, const String &bin);


    CV_EXPORTS_W Net readNetFromONNX(const String &onnxFile);


    CV_EXPORTS Net readNetFromONNX(const char* buffer, size_t sizeBuffer);


    CV_EXPORTS_W Net readNetFromONNX(const std::vector<uchar>& buffer);


    CV_EXPORTS_W Mat readTensorFromONNX(const String& path);


    CV_EXPORTS_W Mat blobFromImage(InputArray image, double scalefactor=1.0, const Size& size = Size(),
                                   const Scalar& mean = Scalar(), bool swapRB=false, bool crop=false,
                                   int ddepth=CV_32F);


    CV_EXPORTS void blobFromImage(InputArray image, OutputArray blob, double scalefactor=1.0,
                                  const Size& size = Size(), const Scalar& mean = Scalar(),
                                  bool swapRB=false, bool crop=false, int ddepth=CV_32F);



    CV_EXPORTS_W Mat blobFromImages(InputArrayOfArrays images, double scalefactor=1.0,
                                    Size size = Size(), const Scalar& mean = Scalar(), bool swapRB=false, bool crop=false,
                                    int ddepth=CV_32F);


    CV_EXPORTS void blobFromImages(InputArrayOfArrays images, OutputArray blob,
                                   double scalefactor=1.0, Size size = Size(),
                                   const Scalar& mean = Scalar(), bool swapRB=false, bool crop=false,
                                   int ddepth=CV_32F);


    CV_EXPORTS_W void imagesFromBlob(const cv::Mat& blob_, OutputArrayOfArrays images_);


    CV_EXPORTS_W void shrinkCaffeModel(const String& src, const String& dst,
                                       const std::vector<String>& layersTypes = std::vector<String>());


    CV_EXPORTS_W void writeTextGraph(const String& model, const String& output);


    CV_EXPORTS_W void NMSBoxes(const std::vector<Rect>& bboxes, const std::vector<float>& scores,
                               const float score_threshold, const float nms_threshold,
                               CV_OUT std::vector<int>& indices,
                               const float eta = 1.f, const int top_k = 0);

    CV_EXPORTS_W void NMSBoxes(const std::vector<Rect2d>& bboxes, const std::vector<float>& scores,
                               const float score_threshold, const float nms_threshold,
                               CV_OUT std::vector<int>& indices,
                               const float eta = 1.f, const int top_k = 0);

    CV_EXPORTS_AS(NMSBoxesRotated) void NMSBoxes(const std::vector<RotatedRect>& bboxes, const std::vector<float>& scores,
                             const float score_threshold, const float nms_threshold,
                             CV_OUT std::vector<int>& indices,
                             const float eta = 1.f, const int top_k = 0);


CV__DNN_INLINE_NS_END
}
}

#include <opencv2/dnn/layer.hpp>
#include <opencv2/dnn/dnn.inl.hpp>


#include <opencv2/dnn/utils/inference_engine.hpp>

#endif
