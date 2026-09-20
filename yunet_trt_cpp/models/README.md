# models

本目录保存：

```
face_detection_yunet_2023mar.onnx   # 原始 ONNX 模型，仅用于构建/重建引擎
yunet_fp16.engine                    # TensorRT 序列化引擎，推理时使用
```

**ONNX 不是推理必须项**：推理只需 `yunet_fp16.engine`。ONNX 仅在首次构建或换机 / 换 TRT 版本
重建引擎时使用；engine 已存在时程序直接加载，不会读取 ONNX。

重建示例：

```bash
./build/yunet_camera_demo --rebuild
```

注意：engine 不能跨 TensorRT 版本 / GPU 架构复用，换机或升级 TRT 后需用 ONNX 重建。
