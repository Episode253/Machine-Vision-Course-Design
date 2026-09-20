# YuNet 人脸检测 C++ TensorRT（核心库 + 示例）

在 Jetson Nano 上用**原生 TensorRT** 接口运行 YuNet ONNX 模型，做**摄像头实时人脸检测**，
输出人脸框、5 个关键点（右眼/左眼/鼻尖/右嘴角/左嘴角）、置信度，
以及**由关键点推算的左右眼 ROI**。

项目采用 **核心库 + 示例** 的结构：

- **核心库 `yunet_trt`**：封装 TensorRT 引擎构建/加载、预处理、解码、NMS，对外只暴露
  `YuNetTRT`（检测器）与 `computeEyeRois`（眼眶推算）两个接口。
- **示例 `examples/camera_demo.cpp`**：演示如何调用核心库，接 OpenCV 摄像头实时显示。

- 推理：TensorRT 7.1.3 + CUDA 10.2，不经过 `cv::dnn`，也不使用 `cv::FaceDetectorYN`。
- 后处理：`cls/obj/bbox/kps` 多尺度解码 + NMS 由核心库自行实现，
  公式与 OpenCV `face_detect.cpp` 保持一致，便于对照学习。

## 目录结构

```
yunet_trt_cpp/
├── CMakeLists.txt              # 构建核心库 + 示例
├── README.md
├── include/
│   └── yunet_trt.hpp           # 【核心库对外接口】FaceResult / YuNetTRT / computeEyeRois
├── src/
│   └── yunet_trt.cpp           # 【核心库实现】引擎构建/加载 + 预处理 + 解码 + NMS
├── examples/
│   └── camera_demo.cpp         # 【示例】调用核心库：摄像头实时人脸 + 双眼 ROI
├── models/
│   ├── face_detection_yunet_2023mar.onnx   # 原始模型 (仅重建引擎时需要，推理非必须)
│   └── yunet_fp16.engine                    # TensorRT 序列化引擎
├── images/                     # 可选的测试图片
└── scripts/
    ├── build.sh                # cmake + make
    └── build_engine.sh         # 可选：用 trtexec 预生成 engine
```

## 依赖

Jetson Nano (JetPack 4.x) 自带：

| 组件      | 版本 (本机实测) |
| --------- | --------------- |
| TensorRT  | 7.1.3           |
| CUDA      | 10.2            |
| OpenCV    | 4.1.1           |
| CMake     | 3.10            |
| g++       | 7.5.0           |

TensorRT 头文件位于 `/usr/include/aarch64-linux-gnu/`，库位于 `/usr/lib/aarch64-linux-gnu/`。

## 关于模型：ONNX 还是 engine？

- **推理只需要 `.engine`**。`.engine` 是 TensorRT 的序列化引擎，自包含，可直接反序列化加载；
  ONNX 只在**构建引擎**时使用一次，之后推理不再碰它。
- 本项目 `models/` 同时保留 ONNX 和 engine：ONNX 只是为了**方便重建**，**不是推理必须项**。
  运行时若 engine 已存在，程序会直接加载 engine，完全不会读取 ONNX。
- 注意引擎**不可跨 TensorRT 版本 / GPU 架构复用**：换 TRT 版本或换板子必须用 ONNX 重建。
- 若 `models/` 下没有 engine，程序会用 `--onnx` 指定的模型构建并保存；因此只有首次构建 / 重建时才需要 ONNX。

## 构建

```bash
cd ~/yunet_trt_cpp
bash scripts/build.sh
# 产物: build/libyunet_trt.a 与 build/yunet_camera_demo
```

手动等价写法：`mkdir -p build && cd build && cmake .. && make -j$(nproc)`。

## 在你的工程里使用核心库

核心库对外只有 `include/yunet_trt.hpp`，用 `add_subdirectory` 或安装后链接均可：

**方式一：add_subdirectory**

```cmake
add_subdirectory(yunet_trt_cpp)
target_link_libraries(your_app PRIVATE yunet_trt)
```

**方式二：安装后使用**

```bash
cd yunet_trt_cpp && mkdir build && cd build
cmake .. && make && sudo make install     # 安装 libyunet_trt.a 与 yunet_trt.hpp
```

**最小调用示例**（详见 `examples/camera_demo.cpp`）：

```cpp
#include "yunet_trt.hpp"

YuNetTRT detector;
detector.init("models/face_detection_yunet_2023mar.onnx",   // ONNX（首次构建引擎用）
              "models/yunet_fp16.engine",                    // engine（存在则直接加载）
              /*useFp16=*/true, /*forceRebuild=*/false);

std::vector<FaceResult> faces = detector.detect(bgrImage);   // bgrImage: CV_8UC3(BGR)
for (const auto& f : faces) {
    f.box;            // 人脸框 (x, y, w, h)
    f.landmarks[0];   // 右眼   landmarks[1] 左眼   ... 共 5 个关键点
    f.score;          // 置信度

    cv::Rect2f eyes[2];
    computeEyeRois(f, eyes, 0.6f);   // eyes[0]=右眼, eyes[1]=左眼
}
```

> 说明：核心库需要 OpenCV 的 `core`、`imgproc`；示例额外用到 `videoio`、`highgui`、`imgcodecs`。

## 运行示例

```bash
# 打开默认摄像头 0，实时检测并显示
./build/yunet_camera_demo

# 指定摄像头 / 分辨率 / 阈值
./build/yunet_camera_demo --camera 0 --width 1280 --height 720 --score 0.6

# 调节眼框大小（边长 = eye-scale * 双眼间距）
./build/yunet_camera_demo --eye-scale 0.5

# 周期打印检测结果（调试 / 无界面观察）
./build/yunet_camera_demo --log-every 30

# 无窗口自检（Jetson 无显示器时），处理 100 帧并保存一张截图
./build/yunet_camera_demo --headless --max-frames 100 --snapshot cam.jpg

# 用 ONNX 重建引擎（换机 / 换 TRT 版本时）
./build/yunet_camera_demo --rebuild
./build/yunet_camera_demo --rebuild --no-fp16   # 重建 FP32 引擎
```

窗口中按键：`q` / `ESC` 退出，`s` 保存当前带标注画面。

> 程序对 `models/...` 做了路径回退：在项目根目录、`build/` 目录（或再上一级）启动都能找到模型，
> 无需手动改路径。也可用 `--onnx`/`--engine` 显式指定。

参数说明：

| 参数               | 说明                                   | 默认 |
| ------------------ | -------------------------------------- | ---- |
| `--onnx <path>`    | ONNX 模型路径（仅构建引擎时需要）      | `models/face_detection_yunet_2023mar.onnx` |
| `--engine <path>`  | 序列化引擎路径（不存在则构建后保存）   | `models/yunet_fp16.engine` |
| `--camera <int>`   | 摄像头编号                             | `0` |
| `--width/--height` | 采集分辨率                             | `1280x720` |
| `--backend <s>`    | 摄像头后端 `auto|dshow|msmf|v4l2`      | `auto` |
| `--score`          | 置信度阈值                             | `0.6` |
| `--nms`            | NMS IoU 阈值                           | `0.3` |
| `--topk`           | NMS 前保留候选框数量                   | `5000` |
| `--log-every <n>`  | 每 n 帧打印一次检测结果，0 表示不打印  | `0` |
| `--eye-scale <f>`  | 眼框边长 = `eye-scale` × 双眼间距      | `0.6` |
| `--no-eyes`        | 不画左右眼 ROI                         | 画 |
| `--no-fp16`        | 关闭 FP16                              | 开启 |
| `--rebuild`        | 强制重建引擎                           | 关闭 |
| `--headless`       | 不显示窗口（自检用）                   | 关闭 |
| `--max-frames <n>` | 处理 n 帧后退出，0 表示一直运行        | `0` |
| `--snapshot <p>`   | 保存一张带标注的截图                   | 空 |

## 左右眼 ROI 的由来

YuNet 本身**只输出人脸框和 5 个关键点，没有眼睛框**。左右眼 ROI 由关键点推算：

```
d = 双眼间距(右眼点与左眼点欧氏距离)
s = eye_scale * d                       # 眼框边长
右眼框 = 以右眼点为中心、边长 s 的正方形
左眼框 = 以左眼点为中心、边长 s 的正方形
```

实现见 `computeEyeRois()`（`include/yunet_trt.hpp`、`src/yunet_trt.cpp`）。
关键点顺序：右眼、左眼、鼻尖、右嘴角、左嘴角。

## 处理流程

```
摄像头取帧 (BGR)
  └─ letterbox 缩放并补边到 640x640（保持长宽比，避免 16:9 变形漏检）
      └─ 构造 NCHW float blob：BGR->RGB，数值保持 0-255，不归一化
          └─ cudaMemcpy 到显存 -> enqueueV2 -> 拷回 12 个输出张量
              └─ 解码: score = sqrt(cls * obj)
                       cx = (c + dx) * stride,  cy = (r + dy) * stride
                       w  = exp(dw) * stride,   h  = exp(dh) * stride
                       landmarks = (kps + 网格坐标) * stride
                  └─ NMS (按 score 降序 + IoU 抑制 + topK)
                      └─ 坐标按 letterbox 比例还原到原图并裁剪
                          └─ 画人脸框/关键点 -> 由关键点算左右眼 ROI -> 显示
```

> 预处理与 OpenCV `FaceDetectorYN` 一致：`swapRB=true`（BGR→RGB）、`scalefactor=1.0`、无均值，
> 即送入网络的是 0–255 的 RGB 浮点值。
