

#ifndef OPENCV_DNN_DNN_ALL_LAYERS_HPP
#define OPENCV_DNN_DNN_ALL_LAYERS_HPP
#include <opencv2/dnn.hpp>

namespace cv {
namespace dnn {
CV__DNN_INLINE_NS_BEGIN





    class CV_EXPORTS BlankLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams &params);
    };


    class CV_EXPORTS ConstLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams &params);
    };


    class CV_EXPORTS LSTMLayer : public Layer
    {
    public:

        static Ptr<LSTMLayer> create(const LayerParams& params);


        CV_DEPRECATED virtual void setWeights(const Mat &Wh, const Mat &Wx, const Mat &b) = 0;


        virtual void setOutShape(const MatShape &outTailShape = MatShape()) = 0;


        CV_DEPRECATED virtual void setUseTimstampsDim(bool use = true) = 0;


        CV_DEPRECATED virtual void setProduceCellOutput(bool produce = false) = 0;



        int inputNameToIndex(String inputName) CV_OVERRIDE;
        int outputNameToIndex(const String& outputName) CV_OVERRIDE;
    };


    class CV_EXPORTS RNNLayer : public Layer
    {
    public:

        static Ptr<RNNLayer> create(const LayerParams& params);


        virtual void setWeights(const Mat &Wxh, const Mat &bh, const Mat &Whh, const Mat &Who, const Mat &bo) = 0;


        virtual void setProduceHiddenOutput(bool produce = false) = 0;

    };

    class CV_EXPORTS BaseConvolutionLayer : public Layer
    {
    public:
        CV_DEPRECATED_EXTERNAL Size kernel, stride, pad, dilation, adjustPad;
        std::vector<size_t> adjust_pads;
        std::vector<size_t> kernel_size, strides, dilations;
        std::vector<size_t> pads_begin, pads_end;
        String padMode;
        int numOutput;
    };

    class CV_EXPORTS ConvolutionLayer : public BaseConvolutionLayer
    {
    public:
        static Ptr<BaseConvolutionLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS DeconvolutionLayer : public BaseConvolutionLayer
    {
    public:
        static Ptr<BaseConvolutionLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS LRNLayer : public Layer
    {
    public:
        int type;

        int size;
        float alpha, beta, bias;
        bool normBySize;

        static Ptr<LRNLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS PoolingLayer : public Layer
    {
    public:
        int type;
        std::vector<size_t> kernel_size, strides;
        std::vector<size_t> pads_begin, pads_end;
        CV_DEPRECATED_EXTERNAL Size kernel, stride, pad;
        CV_DEPRECATED_EXTERNAL int pad_l, pad_t, pad_r, pad_b;
        bool globalPooling;
        bool computeMaxIdx;
        String padMode;
        bool ceilMode;



        bool avePoolPaddedArea;

        Size pooledSize;
        float spatialScale;

        int psRoiOutChannels;

        static Ptr<PoolingLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS SoftmaxLayer : public Layer
    {
    public:
        bool logSoftMax;

        static Ptr<SoftmaxLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS InnerProductLayer : public Layer
    {
    public:
        int axis;
        static Ptr<InnerProductLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS MVNLayer : public Layer
    {
    public:
        float eps;
        bool normVariance, acrossChannels;

        static Ptr<MVNLayer> create(const LayerParams& params);
    };



    class CV_EXPORTS ReshapeLayer : public Layer
    {
    public:
        MatShape newShapeDesc;
        Range newShapeRange;

        static Ptr<ReshapeLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS FlattenLayer : public Layer
    {
    public:
        static Ptr<FlattenLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS ConcatLayer : public Layer
    {
    public:
        int axis;

        bool padding;

        static Ptr<ConcatLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS SplitLayer : public Layer
    {
    public:
        int outputsCount;

        static Ptr<SplitLayer> create(const LayerParams &params);
    };


    class CV_EXPORTS SliceLayer : public Layer
    {
    public:

        std::vector<std::vector<Range> > sliceRanges;
        int axis;
        int num_split;

        static Ptr<SliceLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS PermuteLayer : public Layer
    {
    public:
        static Ptr<PermuteLayer> create(const LayerParams& params);
    };


    class CV_EXPORTS ShuffleChannelLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams& params);

        int group;
    };


    class CV_EXPORTS PaddingLayer : public Layer
    {
    public:
        static Ptr<PaddingLayer> create(const LayerParams& params);
    };


    class CV_EXPORTS ActivationLayer : public Layer
    {
    public:
        virtual void forwardSlice(const float* src, float* dst, int len,
                                  size_t outPlaneSize, int cn0, int cn1) const = 0;
    };

    class CV_EXPORTS ReLULayer : public ActivationLayer
    {
    public:
        float negativeSlope;

        static Ptr<ReLULayer> create(const LayerParams &params);
    };

    class CV_EXPORTS ReLU6Layer : public ActivationLayer
    {
    public:
        float minValue, maxValue;

        static Ptr<ReLU6Layer> create(const LayerParams &params);
    };

    class CV_EXPORTS ChannelsPReLULayer : public ActivationLayer
    {
    public:
        static Ptr<Layer> create(const LayerParams& params);
    };

    class CV_EXPORTS ELULayer : public ActivationLayer
    {
    public:
        static Ptr<ELULayer> create(const LayerParams &params);
    };

    class CV_EXPORTS TanHLayer : public ActivationLayer
    {
    public:
        static Ptr<TanHLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS SigmoidLayer : public ActivationLayer
    {
    public:
        static Ptr<SigmoidLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS BNLLLayer : public ActivationLayer
    {
    public:
        static Ptr<BNLLLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS AbsLayer : public ActivationLayer
    {
    public:
        static Ptr<AbsLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS PowerLayer : public ActivationLayer
    {
    public:
        float power, scale, shift;

        static Ptr<PowerLayer> create(const LayerParams &params);
    };



    class CV_EXPORTS CropLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams &params);
    };

    class CV_EXPORTS EltwiseLayer : public Layer
    {
    public:
        static Ptr<EltwiseLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS BatchNormLayer : public ActivationLayer
    {
    public:
        bool hasWeights, hasBias;
        float epsilon;

        static Ptr<BatchNormLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS MaxUnpoolLayer : public Layer
    {
    public:
        Size poolKernel;
        Size poolPad;
        Size poolStride;

        static Ptr<MaxUnpoolLayer> create(const LayerParams &params);
    };

    class CV_EXPORTS ScaleLayer : public Layer
    {
    public:
        bool hasBias;
        int axis;

        static Ptr<ScaleLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS ShiftLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams& params);
    };

    class CV_EXPORTS PriorBoxLayer : public Layer
    {
    public:
        static Ptr<PriorBoxLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS ReorgLayer : public Layer
    {
    public:
        static Ptr<ReorgLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS RegionLayer : public Layer
    {
    public:
        static Ptr<RegionLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS DetectionOutputLayer : public Layer
    {
    public:
        static Ptr<DetectionOutputLayer> create(const LayerParams& params);
    };


    class CV_EXPORTS NormalizeBBoxLayer : public Layer
    {
    public:
        float pnorm, epsilon;
        CV_DEPRECATED_EXTERNAL bool acrossSpatial;

        static Ptr<NormalizeBBoxLayer> create(const LayerParams& params);
    };


    class CV_EXPORTS ResizeLayer : public Layer
    {
    public:
        static Ptr<ResizeLayer> create(const LayerParams& params);
    };


    class CV_EXPORTS InterpLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams& params);
    };

    class CV_EXPORTS ProposalLayer : public Layer
    {
    public:
        static Ptr<ProposalLayer> create(const LayerParams& params);
    };

    class CV_EXPORTS CropAndResizeLayer : public Layer
    {
    public:
        static Ptr<Layer> create(const LayerParams& params);
    };



CV__DNN_INLINE_NS_END
}
}
#endif
