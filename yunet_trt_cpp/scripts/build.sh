#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j"$(nproc)"
echo "构建完成: build/libyunet_trt.a 与 build/yunet_camera_demo"
