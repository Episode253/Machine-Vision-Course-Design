#ifndef YUNET_TRT_HPP
#define YUNET_TRT_HPP


// yunet_trt —— YuNet 人脸检测的 TensorRT 核心库

// 最小用法：





//                 "models/yunet_fp16.engine");   // 首次构建引擎，之后直接加载




//       const std::array<cv::Rect2f, 2> eyes = computeEyeRois(f);  // [0]=右眼, [1]=左眼


// 本头文件只依赖 OpenCV core；TensorRT/CUDA 细节全部封装在实现里。


#include <array>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/core.hpp>

// 单张人脸的检测结果。
// box 使用原图像素坐标 (x, y, w, h)。
// landmarks 顺序与 OpenCV FaceDetectorYN 一致：
// 0 右眼, 1 左眼, 2 鼻尖, 3 右嘴角, 4 左嘴角。
struct FaceResult {
    cv::Rect2f box;
    std::array<cv::Point2f, 5> landmarks{};
    float score = 0.f;
};

// 由 5 点关键点推算左右眼 ROI：以眼点为中心，边长 = scale * 双眼间距。
// 返回值 [0] = 右眼, [1] = 左眼（与 landmarks 顺序一致）。
[[nodiscard]] std::array<cv::Rect2f, 2> computeEyeRois(const FaceResult& face,
                                                       float scale = 0.6f);

// 基于原生 TensorRT 的 YuNet 人脸检测器。
// OpenCV 仅用于图像读写与结果绘制。
class YuNetTRT {
public:
    YuNetTRT();
    ~YuNetTRT();

    // 引擎持有 GPU 资源，按值移动/复制都不合适，一律禁止。
    YuNetTRT(const YuNetTRT&) = delete;
    YuNetTRT& operator=(const YuNetTRT&) = delete;
    YuNetTRT(YuNetTRT&&) = delete;
    YuNetTRT& operator=(YuNetTRT&&) = delete;

    // 初始化：优先加载 enginePath 处的序列化引擎；
    // 若不存在或 forceRebuild 为真，则从 onnxPath 构建引擎并保存到 enginePath。
    [[nodiscard]] bool init(const std::string& onnxPath,
                            const std::string& enginePath,
                            bool useFp16 = true,
                            bool forceRebuild = false);

    // 对 BGR 图像做人脸检测。
    [[nodiscard]] std::vector<FaceResult> detect(const cv::Mat& bgr,
                                                 float scoreThreshold = 0.9f,
                                                 float nmsThreshold = 0.3f,
                                                 int topK = 5000);

    int inputWidth() const;
    int inputHeight() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
