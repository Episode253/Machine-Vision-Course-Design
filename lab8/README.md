# lab8 综合注视门控与视物变形检测

lab8 将 lab5 的 Amsler 视物变形测试与 lab7 的 YuNet TensorRT 人脸/眼 ROI 检测整合到一起。

## 工作流程

1. `FixationMonitor` 使用 lab7 的 `YuNetTRT` 检测人脸和五点关键点。
2. 通过 `computeEyeRois()` 得到左右眼 ROI。
3. 在两个 ROI 内进行简单暗 pupil 候选检测，计算双眼中心。
4. 首次稳定检测到的双眼中心作为注视基准点。
5. 当前双眼中心与基准点距离小于阈值时，发送“注视有效”。
6. Amsler 测试收到注视无效信号时，当前试次作废并进入等待；注视恢复后才继续下一试次。

## Jetson 构建

lab8 依赖 Jetson 上的 TensorRT、CUDA 和 OpenCV。当前 x86 虚拟机没有 `NvInfer.h`，因此只能验证 Qt 和状态机部分，不能完成真实链接。

在 Jetson 上可以按以下方式构建：

```bash
cd lab8
qmake lab8.pro
make -j4
./lab8
```

如果 TensorRT 不在默认路径，可以设置：

```bash
export TENSORRT_INCLUDE_DIR=/usr/include/aarch64-linux-gnu
export TENSORRT_LIB_DIR=/usr/lib/aarch64-linux-gnu
```

模型默认查找：

```text
yunet_trt_cpp/models/face_detection_yunet_2023mar.onnx
yunet_trt_cpp/models/yunet_fp16.engine
```

摄像头配置仍使用项目统一配置：

```bash
source config/load_visual_lab_config.sh
bash config/run_lab.sh lab8
```

## 需要在 Jetson 上重点调节的参数

- `computeEyeRois(face, 0.6f)` 中的 ROI 缩放系数
- `findPupil()` 的二值化阈值 `55`
- 注视有效距离阈值 `22` 像素
- 摄像头分辨率与实际 YuNet engine 输入尺寸

这些阈值受摄像头距离、光照和镜头视场影响，属于现场标定参数。
