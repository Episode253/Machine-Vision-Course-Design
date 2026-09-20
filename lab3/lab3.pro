QT += core gui widgets
CONFIG += c++17

LAB_WITH_OPENCV = 1
include(../common/common.pri)

SOURCES += \
    $$COMMON_RUNTIME_SOURCES \
    $$COMMON_QT_SOURCES \
    main.cpp \
    mainwindow.cpp \
    low_vision_viewmodel.cpp \
    blur_two/blur_two.cpp \
    visual_field_loss/visual_field_loss.cpp \
    cataract/cataract.cpp \
    glare/glare.cpp

HEADERS += \
    $$COMMON_RUNTIME_HEADERS \
    $$COMMON_QT_HEADERS \
    mainwindow.h \
    low_vision_viewmodel.h \
    blur_two/blur_two.h \
    visual_field_loss/visual_field_loss.h \
    cataract/cataract.h \
    glare/glare.h

INCLUDEPATH += $$COMMON_INCLUDEPATH
