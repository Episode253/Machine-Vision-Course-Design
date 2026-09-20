QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17





LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_CORE_SOURCES \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    capture_source.cpp \
    gray_stage.cpp \
    main.cpp \
    mainwindow.cpp \
    pipeline_viewmodel.cpp \
    register_lab2_stages.cpp \
    sobel_stage.cpp

HEADERS += \
    $$COMMON_CORE_HEADERS \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    capture_source.h \
    gray_stage.h \
    mainwindow.h \
    pipeline_viewmodel.h \
    register_lab2_stages.h \
    sobel_stage.h

INCLUDEPATH += $$COMMON_INCLUDEPATH

FORMS += \
    mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
