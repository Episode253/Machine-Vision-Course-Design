# Visual Lab 配置

这个目录用于统一管理实验室摄像头和运行参数，避免各个 lab 里反复写死设备号和路径。

## 1. 直接修改配置

编辑当前目录下的 `visual_lab_config.json`。

示例：

```json
{
  "camera_index": 0,
  "camera_device": "/dev/video0",
  "camera_backend": "v4l2",
  "frame_width": 640,
  "frame_height": 480,
  "frame_image": "",
  "yunet_onnx": "",
  "yunet_engine": ""
}
```

## 2. 在启动前加载环境变量

在运行任何 lab 前执行：

```bash
source config/load_visual_lab_config.sh
```

它会读取配置文件，并把值导出为环境变量：

- VISUAL_LAB_CAMERA_INDEX
- VISUAL_LAB_CAMERA_DEVICE
- VISUAL_LAB_CAMERA_BACKEND
- VISUAL_LAB_FRAME_WIDTH
- VISUAL_LAB_FRAME_HEIGHT
- VISUAL_LAB_FRAME_IMAGE
- VISUAL_LAB_YUNET_ONNX
- VISUAL_LAB_YUNET_ENGINE
- OPENCV_ROOT
- OPENCV_INCLUDE_DIR
- OPENCV_LIB_DIR
- TENSORRT_INCLUDE_DIR
- TENSORRT_LIB_DIR

## 3. 运行方式

例如：

```bash
source config/load_visual_lab_config.sh
cd lab1
./lab1
```

如果你想临时覆盖配置，可以在启动前再设：

```bash
export VISUAL_LAB_CAMERA_DEVICE=/dev/video2
```

环境变量优先级高于 JSON 文件。

## Jetson ARM64 配置

OpenCV 的架构适配发生在 qmake/build 阶段，因此要在执行 qmake 或启动脚本前加载配置。
Jetson 上可以将路径写入 JSON：

```json
{
  "opencv_root": "/usr",
  "opencv_include_dir": "/usr/include/aarch64-linux-gnu",
  "opencv_lib_dir": "/usr/lib/aarch64-linux-gnu",
  "tensorrt_include_dir": "/usr/include/aarch64-linux-gnu",
  "tensorrt_lib_dir": "/usr/lib/aarch64-linux-gnu"
}
```

如果 Jetson 的 OpenCV 通过 pkg-config 安装，OpenCV 三项可以留空，`common.pri` 会回退到系统 `opencv4` 配置；TensorRT 两项仍建议填写实际路径。
