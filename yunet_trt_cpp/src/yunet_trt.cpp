#include "yunet_trt.hpp"

#include <NvInfer.h>
#include <NvInferRuntime.h>
#include <NvOnnxParser.h>
#include <cuda_fp16.h>
#include <cuda_runtime_api.h>

#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

namespace {

// TensorRT 对象用 destroy() 释放（本工程编译所依赖的 API）；用 unique_ptr
// 包起来后所有权就是 RAII，不再需要手写 release()。
struct TrtDestroy {
    template <class T>
    void operator()(T* object) const noexcept
    {
        if (object)
            object->destroy();
    }
};

struct CudaFree {
    void operator()(void* pointer) const noexcept
    {
        if (pointer)
            cudaFree(pointer);
    }
};

// cudaStream_t 是不透明指针类型，用一个小 RAII 包装比 unique_ptr + remove_pointer 清晰。
class CudaStream {
public:
    CudaStream() = default;
    ~CudaStream() { reset(); }
    CudaStream(const CudaStream&) = delete;
    CudaStream& operator=(const CudaStream&) = delete;

    bool create()
    {
        reset();
        return cudaStreamCreate(&m_handle) == cudaSuccess;
    }

    void reset()
    {
        if (m_handle) {
            cudaStreamDestroy(m_handle);
            m_handle = nullptr;
        }
    }

    cudaStream_t get() const { return m_handle; }

private:
    cudaStream_t m_handle = nullptr;
};

constexpr const char* kInputName = "input";
constexpr std::size_t kStrideCount = 3;
constexpr int kChannels = 3;
constexpr int kDefaultProfileSide = 640;
constexpr std::size_t kWorkspaceBytes = 1ULL << 28;
constexpr std::array<int, kStrideCount> kStrides{8, 16, 32};
constexpr std::array<const char*, kStrideCount> kClsNames{"cls_8", "cls_16", "cls_32"};
constexpr std::array<const char*, kStrideCount> kObjNames{"obj_8", "obj_16", "obj_32"};
constexpr std::array<const char*, kStrideCount> kBboxNames{"bbox_8", "bbox_16", "bbox_32"};
constexpr std::array<const char*, kStrideCount> kKpsNames{"kps_8", "kps_16", "kps_32"};

class Logger final : public nvinfer1::ILogger {
public:
    void log(Severity severity, const char* msg) noexcept override
    {
        if (severity <= Severity::kWARNING)
            std::cerr << "[TensorRT] " << msg << '\n';
    }
};

// 张量元素数；遇到未解析的动态维（<= 0）返回 0，调用方据此报错。
std::size_t volume(const nvinfer1::Dims& dims)
{
    if (dims.nbDims <= 0)
        return 0;
    std::size_t total = 1;
    for (int i = 0; i < dims.nbDims; ++i) {
        const auto dim = static_cast<std::int64_t>(dims.d[i]);
        if (dim <= 0)
            return 0;
        total *= static_cast<std::size_t>(dim);
    }
    return total;
}

// 单个元素字节数；未知类型返回 0。
std::size_t elementSize(nvinfer1::DataType type)
{
    switch (type) {
    case nvinfer1::DataType::kFLOAT:
        return sizeof(float);
    case nvinfer1::DataType::kHALF:
        return sizeof(__half);
    case nvinfer1::DataType::kINT8:
        return sizeof(std::int8_t);
    case nvinfer1::DataType::kINT32:
        return sizeof(std::int32_t);
    case nvinfer1::DataType::kBOOL:
        return sizeof(bool);
    default:
        return 0;
    }
}

bool readFile(const std::string& path, std::vector<char>& data)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return false;
    file.seekg(0, std::ios::end);
    const std::streamoff size = file.tellg();
    if (size < 0)
        return false;
    file.seekg(0, std::ios::beg);
    data.resize(static_cast<std::size_t>(size));
    if (size > 0)
        file.read(data.data(), static_cast<std::streamsize>(size));
    return static_cast<bool>(file);
}

bool writeFile(const std::string& path, const void* data, std::size_t size)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
        return false;
    file.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
    return static_cast<bool>(file);
}

float iou(const cv::Rect2f& a, const cv::Rect2f& b)
{
    const float x1 = std::max(a.x, b.x);
    const float y1 = std::max(a.y, b.y);
    const float x2 = std::min(a.x + a.width, b.x + b.width);
    const float y2 = std::min(a.y + a.height, b.y + b.height);
    const float w = std::max(0.f, x2 - x1);
    const float h = std::max(0.f, y2 - y1);
    const float inter = w * h;
    const float uni = a.width * a.height + b.width * b.height - inter;
    return uni > 0.f ? inter / uni : 0.f;
}

}

struct YuNetTRT::Impl {
    Logger logger;
    // 声明顺序即“逆序析构顺序”：先释放 device 缓冲，再销毁 stream/context/engine/runtime。
    std::unique_ptr<nvinfer1::IRuntime, TrtDestroy> runtime;
    std::unique_ptr<nvinfer1::ICudaEngine, TrtDestroy> engine;
    std::unique_ptr<nvinfer1::IExecutionContext, TrtDestroy> context;
    CudaStream stream;

    int inputIndex = -1;
    int inputW = 0;
    int inputH = 0;
    bool dynamic = false;

    struct Binding {
        int index = -1;
        nvinfer1::DataType dtype = nvinfer1::DataType::kFLOAT;
        std::size_t count = 0;
        std::size_t bytes = 0;
        std::unique_ptr<void, CudaFree> device;
    };
    std::vector<Binding> bindings;
    // outputIndex[0..3][0..2] = cls/obj/bbox/kps 在 bindings 中的下标
    std::array<std::array<int, kStrideCount>, 4> outputIndex{};

    bool init(const std::string& onnxPath, const std::string& enginePath,
              bool useFp16, bool forceRebuild);
    bool buildEngine(const std::string& onnxPath, bool useFp16);
    bool loadEngine(const std::string& enginePath);
    bool setup();

    std::vector<FaceResult> detect(const cv::Mat& bgr, float scoreThreshold,
                                   float nmsThreshold, int topK);
};

bool YuNetTRT::Impl::buildEngine(const std::string& onnxPath, bool useFp16)
{
    std::unique_ptr<nvinfer1::IBuilder, TrtDestroy> builder{nvinfer1::createInferBuilder(logger)};
    if (!builder) {
        std::cerr << "createInferBuilder 失败\n";
        return false;
    }
    const std::uint32_t flags =
        1U << static_cast<std::uint32_t>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
    std::unique_ptr<nvinfer1::INetworkDefinition, TrtDestroy> network{
        builder->createNetworkV2(flags)};
    if (!network) {
        std::cerr << "createNetworkV2 失败\n";
        return false;
    }
    std::unique_ptr<nvinfer1::IBuilderConfig, TrtDestroy> config{builder->createBuilderConfig()};
    if (!config) {
        std::cerr << "createBuilderConfig 失败\n";
        return false;
    }
    std::unique_ptr<nvonnxparser::IParser, TrtDestroy> parser{
        nvonnxparser::createParser(*network, logger)};
    if (!parser) {
        std::cerr << "createParser 失败\n";
        return false;
    }

    const bool parsed =
        parser->parseFromFile(onnxPath.c_str(),
                              static_cast<int>(nvinfer1::ILogger::Severity::kWARNING));
    if (!parsed || network->getNbInputs() == 0) {
        std::cerr << "ONNX 解析失败（文件不存在或不是有效模型）: " << onnxPath << '\n';
        return false;
    }

    config->setMaxWorkspaceSize(kWorkspaceBytes);
    if (useFp16 && builder->platformHasFastFp16()) {
        config->setFlag(nvinfer1::BuilderFlag::kFP16);
        std::cout << "[YuNetTRT] 启用 FP16\n";
    }

    // 兼容动态输入：若维度含 -1，则固定一个 640x640 的优化 profile。
    nvinfer1::ITensor* input = network->getInput(0);
    if (!input) {
        std::cerr << "网络没有输入张量\n";
        return false;
    }
    nvinfer1::Dims dims = input->getDimensions();
    dynamic = false;
    for (int i = 0; i < dims.nbDims; ++i)
        if (dims.d[i] < 0)
            dynamic = true;
    if (dynamic) {
        nvinfer1::Dims profile = dims;
        for (int i = 0; i < profile.nbDims; ++i) {
            if (profile.d[i] < 0) {
                if (i == 0)
                    profile.d[i] = 1;
                else if (i == 1)
                    profile.d[i] = kChannels;
                else
                    profile.d[i] = kDefaultProfileSide;
            }
        }
        nvinfer1::IOptimizationProfile* optProfile = builder->createOptimizationProfile();
        if (!optProfile) {
            std::cerr << "createOptimizationProfile 失败\n";
            return false;
        }
        optProfile->setDimensions(input->getName(), nvinfer1::OptProfileSelector::kMIN, profile);
        optProfile->setDimensions(input->getName(), nvinfer1::OptProfileSelector::kOPT, profile);
        optProfile->setDimensions(input->getName(), nvinfer1::OptProfileSelector::kMAX, profile);
        config->addOptimizationProfile(optProfile); // 所有权归 config，不要手动释放
        std::cout << "[YuNetTRT] 动态输入，固定 profile 为 " << profile.d[profile.nbDims - 1] << "x"
                  << profile.d[profile.nbDims - 2] << '\n';
    }

    engine.reset(builder->buildEngineWithConfig(*network, *config));
    if (!engine) {
        std::cerr << "构建 TensorRT 引擎失败\n";
        return false;
    }
    return true;
}

bool YuNetTRT::Impl::loadEngine(const std::string& enginePath)
{
    std::vector<char> data;
    if (!readFile(enginePath, data)) {
        // 引擎不存在属于正常情况（首次运行会改用 ONNX 构建），不打印错误。
        return false;
    }
    runtime.reset(nvinfer1::createInferRuntime(logger));
    if (!runtime)
        return false;
    engine.reset(runtime->deserializeCudaEngine(data.data(), data.size(), nullptr));
    if (!engine) {
        std::cerr << "反序列化引擎失败: " << enginePath << '\n';
        return false;
    }
    return true;
}

bool YuNetTRT::Impl::setup()
{
    if (!runtime)
        runtime.reset(nvinfer1::createInferRuntime(logger));
    if (!runtime || !engine)
        return false;

    context.reset(engine->createExecutionContext());
    if (!context) {
        std::cerr << "createExecutionContext 失败\n";
        return false;
    }
    if (!stream.create()) {
        std::cerr << "cudaStreamCreate 失败\n";
        return false;
    }

    inputIndex = engine->getBindingIndex(kInputName);
    if (inputIndex < 0) {
        for (int i = 0; i < engine->getNbBindings(); ++i)
            if (engine->bindingIsInput(i)) {
                inputIndex = i;
                break;
            }
    }
    if (inputIndex < 0) {
        std::cerr << "找不到网络输入\n";
        return false;
    }

    nvinfer1::Dims inDims = context->getBindingDimensions(inputIndex);
    for (int i = 0; i < inDims.nbDims; ++i) {
        if (inDims.d[i] < 0) {
            if (i == 0)
                inDims.d[i] = 1;
            else if (i == 1)
                inDims.d[i] = kChannels;
            else
                inDims.d[i] = kDefaultProfileSide;
        }
    }
    if (dynamic) {
        if (!context->setBindingDimensions(inputIndex, inDims)) {
            std::cerr << "setBindingDimensions 失败\n";
            return false;
        }
        inDims = context->getBindingDimensions(inputIndex);
    }
    if (!context->allInputDimensionsSpecified()) {
        std::cerr << "网络输入维度未完全确定\n";
        return false;
    }
    inputH = static_cast<int>(inDims.d[inDims.nbDims - 2]);
    inputW = static_cast<int>(inDims.d[inDims.nbDims - 1]);
    if (inputW <= 0 || inputH <= 0) {
        std::cerr << "非法的网络输入尺寸\n";
        return false;
    }
    std::cout << "[YuNetTRT] 网络输入: " << inputW << "x" << inputH << '\n';

    const int bindingCount = engine->getNbBindings();
    bindings.resize(static_cast<std::size_t>(bindingCount));
    for (int i = 0; i < bindingCount; ++i) {
        Binding& binding = bindings[static_cast<std::size_t>(i)];
        binding.index = i;
        binding.dtype = engine->getBindingDataType(i);
        binding.count = volume(context->getBindingDimensions(i));
        const std::size_t element = elementSize(binding.dtype);
        if (binding.count == 0 || element == 0) {
            std::cerr << "binding " << i << " 的维度或数据类型无效\n";
            return false;
        }
        binding.bytes = binding.count * element;
        void* device = nullptr;
        if (cudaMalloc(&device, binding.bytes) != cudaSuccess) {
            std::cerr << "cudaMalloc 失败 (binding " << i << ")\n";
            return false;
        }
        binding.device.reset(device);
    }

    for (std::size_t strideIndex = 0; strideIndex < kStrideCount; ++strideIndex) {
        outputIndex[0][strideIndex] = engine->getBindingIndex(kClsNames[strideIndex]);
        outputIndex[1][strideIndex] = engine->getBindingIndex(kObjNames[strideIndex]);
        outputIndex[2][strideIndex] = engine->getBindingIndex(kBboxNames[strideIndex]);
        outputIndex[3][strideIndex] = engine->getBindingIndex(kKpsNames[strideIndex]);
        if (outputIndex[0][strideIndex] < 0 || outputIndex[1][strideIndex] < 0 ||
            outputIndex[2][strideIndex] < 0 || outputIndex[3][strideIndex] < 0) {
            std::cerr << "缺少 YuNet 输出张量 (stride " << kStrides[strideIndex] << ")\n";
            return false;
        }
    }
    return true;
}

bool YuNetTRT::Impl::init(const std::string& onnxPath, const std::string& enginePath,
                          bool useFp16, bool forceRebuild)
{
    bool loaded = false;
    if (!forceRebuild && !enginePath.empty()) {
        loaded = loadEngine(enginePath);
        if (loaded)
            std::cout << "[YuNetTRT] 已加载序列化引擎: " << enginePath << '\n';
    }
    if (!loaded) {
        if (!buildEngine(onnxPath, useFp16))
            return false;
        if (!enginePath.empty()) {
            std::unique_ptr<nvinfer1::IHostMemory, TrtDestroy> serialized{engine->serialize()};
            if (serialized) {
                writeFile(enginePath, serialized->data(), serialized->size());
                std::cout << "[YuNetTRT] 引擎已保存: " << enginePath << " ("
                          << serialized->size() << " bytes)\n";
            }
        }
    }
    return setup();
}

std::vector<FaceResult> YuNetTRT::Impl::detect(const cv::Mat& bgr, float scoreThreshold,
                                               float nmsThreshold, int topK)
{
    std::vector<FaceResult> faces;
    if (!context || bgr.empty())
        return faces;

    cv::Mat image;
    if (bgr.channels() == 1)
        cv::cvtColor(bgr, image, cv::COLOR_GRAY2BGR);
    else if (bgr.channels() == 4)
        cv::cvtColor(bgr, image, cv::COLOR_BGRA2BGR);
    else
        image = bgr;

    const int origW = image.cols;
    const int origH = image.rows;

    // 预处理：letterbox 缩放（保持长宽比，右侧/下方补 0），构造 NCHW float blob。
    // 与 OpenCV FaceDetectorYN 一致：BGR->RGB（swapRB），数值保持 0-255，不做归一化。
    // 若直接拉伸到 640x640，16:9 画面会严重变形导致漏检。
    const float scale =
        std::min(static_cast<float>(inputW) / origW, static_cast<float>(inputH) / origH);
    const int newW = std::max(1, static_cast<int>(std::lround(origW * scale)));
    const int newH = std::max(1, static_cast<int>(std::lround(origH * scale)));
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(newW, newH));
    cv::Mat canvas(inputH, inputW, CV_8UC3, cv::Scalar(0, 0, 0));
    resized.copyTo(canvas(cv::Rect(0, 0, newW, newH)));

    std::vector<float> blob(static_cast<std::size_t>(3) * inputH * inputW);
    const std::size_t plane = static_cast<std::size_t>(inputH) * inputW;
    float* chR = blob.data();
    float* chG = blob.data() + plane;
    float* chB = blob.data() + 2 * plane;
    for (int y = 0; y < inputH; ++y) {
        const uchar* row = canvas.ptr<uchar>(y);
        for (int x = 0; x < inputW; ++x) {
            const uchar* p = row + x * 3;
            const std::size_t idx = static_cast<std::size_t>(y) * inputW + x;
            chR[idx] = p[2];
            chG[idx] = p[1];
            chB[idx] = p[0];
        }
    }

    Binding& inputBinding = bindings[static_cast<std::size_t>(inputIndex)];
    if (inputBinding.dtype == nvinfer1::DataType::kFLOAT) {
        cudaMemcpyAsync(inputBinding.device.get(), blob.data(), blob.size() * sizeof(float),
                        cudaMemcpyHostToDevice, stream.get());
    } else {
        std::vector<__half> halfBlob(blob.size());
        for (std::size_t i = 0; i < blob.size(); ++i)
            halfBlob[i] = __float2half(blob[i]);
        cudaMemcpyAsync(inputBinding.device.get(), halfBlob.data(),
                        halfBlob.size() * sizeof(__half), cudaMemcpyHostToDevice, stream.get());
    }

    std::vector<void*> pointers(bindings.size(), nullptr);
    for (const auto& binding : bindings)
        pointers[static_cast<std::size_t>(binding.index)] = binding.device.get();
    if (!context->enqueueV2(pointers.data(), stream.get(), nullptr)) {
        std::cerr << "TensorRT enqueueV2 失败\n";
        return faces;
    }
    cudaStreamSynchronize(stream.get());

    // 拷回 12 个输出张量，顺序为 cls/obj/bbox/kps × stride{8,16,32}。
    std::vector<std::vector<float>> out(4 * kStrideCount);
    for (std::size_t category = 0; category < 4; ++category) {
        for (std::size_t strideIndex = 0; strideIndex < kStrideCount; ++strideIndex) {
            const int bindingIndex = outputIndex[category][strideIndex];
            if (bindingIndex < 0 || bindingIndex >= static_cast<int>(bindings.size()))
                continue;
            Binding& binding = bindings[static_cast<std::size_t>(bindingIndex)];
            std::vector<float>& dst = out[category * kStrideCount + strideIndex];
            dst.resize(binding.count);
            if (binding.dtype == nvinfer1::DataType::kFLOAT) {
                cudaMemcpyAsync(dst.data(), binding.device.get(), binding.count * sizeof(float),
                                cudaMemcpyDeviceToHost, stream.get());
            } else {
                std::vector<__half> halfOutput(binding.count);
                cudaMemcpyAsync(halfOutput.data(), binding.device.get(),
                                binding.count * sizeof(__half), cudaMemcpyDeviceToHost,
                                stream.get());
                cudaStreamSynchronize(stream.get());
                for (std::size_t i = 0; i < binding.count; ++i)
                    dst[i] = __half2float(halfOutput[i]);
            }
        }
    }
    cudaStreamSynchronize(stream.get());

    // 解码：score = sqrt(cls * obj)，框与关键点按 stride 还原。
    for (std::size_t strideIndex = 0; strideIndex < kStrideCount; ++strideIndex) {
        const int stride = kStrides[strideIndex];
        const int cols = inputW / stride;
        const int rows = inputH / stride;
        const std::vector<float>& cls = out[0 * kStrideCount + strideIndex];
        const std::vector<float>& obj = out[1 * kStrideCount + strideIndex];
        const std::vector<float>& bbox = out[2 * kStrideCount + strideIndex];
        const std::vector<float>& kps = out[3 * kStrideCount + strideIndex];

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                const std::size_t idx = static_cast<std::size_t>(r) * cols + c;
                const float cs = std::min(1.f, std::max(0.f, cls[idx]));
                const float os = std::min(1.f, std::max(0.f, obj[idx]));
                const float score = std::sqrt(cs * os);
                if (score < scoreThreshold)
                    continue;

                const float cx = (c + bbox[idx * 4 + 0]) * stride;
                const float cy = (r + bbox[idx * 4 + 1]) * stride;
                const float w = std::exp(bbox[idx * 4 + 2]) * stride;
                const float h = std::exp(bbox[idx * 4 + 3]) * stride;

                FaceResult face;
                face.box = cv::Rect2f(cx - w / 2.f, cy - h / 2.f, w, h);
                for (std::size_t n = 0; n < face.landmarks.size(); ++n) {
                    face.landmarks[n].x = (kps[idx * 10 + 2 * n] + c) * stride;
                    face.landmarks[n].y = (kps[idx * 10 + 2 * n + 1] + r) * stride;
                }
                face.score = score;
                faces.push_back(face);
            }
        }
    }

    // NMS：先按 score 降序，保留 topK 后再做 IoU 抑制。
    std::sort(faces.begin(), faces.end(),
              [](const FaceResult& a, const FaceResult& b) { return a.score > b.score; });
    if (topK > 0 && faces.size() > static_cast<std::size_t>(topK))
        faces.resize(static_cast<std::size_t>(topK));
    std::vector<FaceResult> kept;
    kept.reserve(faces.size());
    for (std::size_t i = 0; i < faces.size(); ++i) {
        bool keep = true;
        for (const FaceResult& accepted : kept) {
            if (iou(faces[i].box, accepted.box) > nmsThreshold) {
                keep = false;
                break;
            }
        }
        if (keep)
            kept.push_back(faces[i]);
    }
    faces.swap(kept);

    // 映射回原图坐标并裁剪（letterbox 的上/左偏移为 0，两轴用同一 scale）。
    const float invScale = 1.0f / scale;
    for (auto& face : faces) {
        face.box.x *= invScale;
        face.box.y *= invScale;
        face.box.width *= invScale;
        face.box.height *= invScale;
        for (auto& landmark : face.landmarks) {
            landmark.x *= invScale;
            landmark.y *= invScale;
        }
        face.box.x = std::max(0.f, face.box.x);
        face.box.y = std::max(0.f, face.box.y);
        face.box.width = std::max(0.f, std::min(face.box.width, origW - face.box.x));
        face.box.height = std::max(0.f, std::min(face.box.height, origH - face.box.y));
    }
    return faces;
}

YuNetTRT::YuNetTRT()
    : impl_(std::make_unique<Impl>())
{
}

YuNetTRT::~YuNetTRT() = default;

bool YuNetTRT::init(const std::string& onnxPath, const std::string& enginePath,
                    bool useFp16, bool forceRebuild)
{
    return impl_->init(onnxPath, enginePath, useFp16, forceRebuild);
}

std::vector<FaceResult> YuNetTRT::detect(const cv::Mat& bgr, float scoreThreshold,
                                         float nmsThreshold, int topK)
{
    return impl_->detect(bgr, scoreThreshold, nmsThreshold, topK);
}

int YuNetTRT::inputWidth() const
{
    return impl_->inputW;
}

int YuNetTRT::inputHeight() const
{
    return impl_->inputH;
}

std::array<cv::Rect2f, 2> computeEyeRois(const FaceResult& face, float scale)
{
    const float distance = static_cast<float>(cv::norm(face.landmarks[1] - face.landmarks[0]));
    const float side = scale * distance;
    std::array<cv::Rect2f, 2> rois{};
    for (std::size_t i = 0; i < rois.size(); ++i) {
        rois[i] = cv::Rect2f(face.landmarks[i].x - side / 2.f, face.landmarks[i].y - side / 2.f,
                             side, side);
    }
    return rois;
}
