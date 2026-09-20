QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17





LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    calibration_session.cpp \
    calibration_viewmodel.cpp \
    eye_detection.cpp \
    main.cpp \
    mainwindow.cpp \
    measurement_worker.cpp

HEADERS += \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    calibration_session.h \
    calibration_viewmodel.h \
    eye_detection.h \
    mainwindow.h \
    measurement_worker.h

INCLUDEPATH += $$COMMON_INCLUDEPATH

FORMS += \
    mainwindow.ui

RESOURCES += \
    test_images.qrc


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
