cmake_minimum_required(VERSION 3.10)
project(yunet_trt_cpp CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ---- TensorRT (Jetson: headers 位于 /usr/include/aarch64-linux-gnu) ----
find_path(TENSORRT_INCLUDE_DIR NvInfer.h
  PATHS /usr/include/aarch64-linux-gnu /usr/include /usr/local/include)
find_library(NVINFER_LIB nvinfer
  PATHS /usr/lib/aarch64-linux-gnu /usr/lib /usr/local/lib)
find_library(NVONNXPARSER_LIB nvonnxparser
  PATHS /usr/lib/aarch64-linux-gnu /usr/lib /usr/local/lib)

# ---- CUDA runtime (推理只需 cudart) ----
find_path(CUDA_INCLUDE_DIR cuda_runtime_api.h
  PATHS /usr/local/cuda/include /usr/local/cuda-10.2/include /usr/include)
find_library(CUDART_LIB cudart
  PATHS /usr/local/cuda/lib64 /usr/local/cuda-10.2/lib64
        /usr/lib/aarch64-linux-gnu /usr/lib)

# ---- OpenCV ----
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs videoio highgui)

if(NOT TENSORRT_INCLUDE_DIR OR NOT NVINFER_LIB OR NOT NVONNXPARSER_LIB)
  message(FATAL_ERROR "找不到 TensorRT (NvInfer.h / libnvinfer / libnvonnxparser)")
endif()
if(NOT CUDA_INCLUDE_DIR OR NOT CUDART_LIB)
  message(FATAL_ERROR "找不到 CUDA runtime (cuda_runtime_api.h / libcudart)")
endif()

# =============================================================================
# 核心库：只依赖 TensorRT / CUDA / OpenCV 的 core+imgproc
# =============================================================================
add_library(yunet_trt STATIC
  src/yunet_trt.cpp)

target_include_directories(yunet_trt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${TENSORRT_INCLUDE_DIR}
  ${CUDA_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS})

target_link_libraries(yunet_trt PUBLIC
  ${NVINFER_LIB}
  ${NVONNXPARSER_LIB}
  ${CUDART_LIB}
  ${OpenCV_LIBS})

# =============================================================================
# 示例程序：演示如何调用核心库（摄像头实时人脸 + 双眼 ROI）
# =============================================================================
add_executable(yunet_camera_demo
  examples/camera_demo.cpp)

target_link_libraries(yunet_camera_demo PRIVATE
  yunet_trt
  ${OpenCV_LIBS})

# =============================================================================
# 安装：libyunet_trt.a + yunet_trt.hpp，供其他工程 add_subdirectory / find_library 使用
# =============================================================================
include(GNUInstallDirs)
install(TARGETS yunet_trt
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/yunet_trt.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

message(STATUS "TensorRT include: ${TENSORRT_INCLUDE_DIR}")
message(STATUS "OpenCV version  : ${OpenCV_VERSION}")
message(STATUS "产物: libyunet_trt.a + yunet_camera_demo")
