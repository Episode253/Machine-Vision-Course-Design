#!/usr/bin/env bash
# 可选：用 trtexec 预先生成 FP16 序列化引擎。
# 程序首次运行时若没有 engine 也会自动构建，因此本脚本不是必须的。
set -e
cd "$(dirname "$0")/.."
ONNX=${1:-models/face_detection_yunet_2023mar.onnx}
ENGINE=${2:-models/yunet_fp16.engine}
/usr/src/tensorrt/bin/trtexec --onnx="$ONNX" --saveEngine="$ENGINE" --fp16 --buildOnly
echo "引擎已保存: $ENGINE"
