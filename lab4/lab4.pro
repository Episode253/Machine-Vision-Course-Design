QT += core gui widgets
CONFIG += c++17

LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    ../lab3/low_vision_viewmodel.cpp \
    ../lab3/blur_two/blur_two.cpp \
    ../lab3/visual_field_loss/visual_field_loss.cpp \
    ../lab3/cataract/cataract.cpp \
    ../lab3/glare/glare.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    ../lab3/low_vision_viewmodel.h \
    ../lab3/blur_two/blur_two.h \
    ../lab3/visual_field_loss/visual_field_loss.h \
    ../lab3/cataract/cataract.h \
    ../lab3/glare/glare.h \
    mainwindow.h

INCLUDEPATH += $$COMMON_INCLUDEPATH ../lab3
