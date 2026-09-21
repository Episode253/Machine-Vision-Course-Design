QT += core gui widgets
CONFIG += c++17

LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    detector_worker.cpp \
    main.cpp \
    mainwindow.cpp \
    $$PWD/../yunet_trt_cpp/src/yunet_trt.cpp      # ← 新增

HEADERS += \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    detector_worker.h \
    mainwindow.h

INCLUDEPATH += $$COMMON_INCLUDEPATH \
    $$PWD/../yunet_trt_cpp/include \
    /usr/include/aarch64-linux-gnu \              # ← TensorRT 头文件（NvInfer.h）
    /usr/local/cuda/include                       # ← CUDA 头文件

LIBS += \
    -L/usr/lib/aarch64-linux-gnu \
    -L/usr/local/cuda/lib64 \
    -lnvinfer \
    -lnvonnxparser \
    -lcudart