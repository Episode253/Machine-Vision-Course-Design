QT += core gui widgets
CONFIG += c++17

LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    detector_worker.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    detector_worker.h \
    mainwindow.h

INCLUDEPATH += $$COMMON_INCLUDEPATH \
    $$PWD/../yunet_trt_cpp/include
