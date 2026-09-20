QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17





LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_CORE_SOURCES \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    camera_worker.cpp \
    display_viewmodel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    $$COMMON_CORE_HEADERS \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    camera_worker.h \
    display_viewmodel.h \
    mainwindow.h

INCLUDEPATH += $$COMMON_INCLUDEPATH

FORMS += \
    mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
