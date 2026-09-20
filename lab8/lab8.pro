QT += core gui widgets
CONFIG += c++17

LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    ../yunet_trt_cpp/src/yunet_trt.cpp \
    amsler_test.cpp \
    amsler_widget.cpp \
    fixation_monitor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    ../yunet_trt_cpp/include/yunet_trt.hpp \
    amsler_test.h \
    amsler_widget.h \
    fixation_monitor.h \
    mainwindow.h

INCLUDEPATH += $$COMMON_INCLUDEPATH \
    $$PWD/../yunet_trt_cpp/include

TENSORRT_INCLUDE_DIR = $$(TENSORRT_INCLUDE_DIR)
isEmpty(TENSORRT_INCLUDE_DIR):exists(/usr/include/aarch64-linux-gnu/NvInfer.h) {
    TENSORRT_INCLUDE_DIR = /usr/include/aarch64-linux-gnu
}
!isEmpty(TENSORRT_INCLUDE_DIR): INCLUDEPATH += $$TENSORRT_INCLUDE_DIR

TENSORRT_LIB_DIR = $$(TENSORRT_LIB_DIR)
isEmpty(TENSORRT_LIB_DIR):exists(/usr/lib/aarch64-linux-gnu/libnvinfer.so) {
    TENSORRT_LIB_DIR = /usr/lib/aarch64-linux-gnu
}
!isEmpty(TENSORRT_LIB_DIR): LIBS += -L$$TENSORRT_LIB_DIR
LIBS += -lnvinfer -lnvonnxparser -lcudart
