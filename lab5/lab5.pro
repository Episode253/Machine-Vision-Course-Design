QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17





include(../common/common.pri)

SOURCES += \
    $$COMMON_LOGGING_SOURCES \
    amsler_test.cpp \
    amsler_widget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    $$COMMON_LOGGING_HEADERS \
    amsler_test.h \
    amsler_widget.h \
    mainwindow.h

INCLUDEPATH += $$COMMON_INCLUDEPATH

FORMS += \
    mainwindow.ui


qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
