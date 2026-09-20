
























COMMON_PATH = $$PWD
COMMON_INCLUDEPATH = $$COMMON_PATH

COMMON_CORE_HEADERS = \
    $$COMMON_PATH/core/frame.hpp \
    $$COMMON_PATH/core/stage.hpp \
    $$COMMON_PATH/core/bounded_queue.hpp \
    $$COMMON_PATH/core/stage_registry.h

COMMON_CORE_SOURCES = \
    $$COMMON_PATH/core/stage_registry.cpp

COMMON_RUNTIME_HEADERS = \
    $$COMMON_PATH/runtime/worker_thread.h \
    $$COMMON_PATH/runtime/pipeline.h \
    $$COMMON_PATH/runtime/frame_source.h

COMMON_RUNTIME_SOURCES = \
    $$COMMON_PATH/runtime/worker_thread.cpp \
    $$COMMON_PATH/runtime/pipeline.cpp \
    $$COMMON_PATH/runtime/frame_source.cpp

COMMON_LOGGING_HEADERS = \
    $$COMMON_PATH/qt/logging.h

COMMON_LOGGING_SOURCES = \
    $$COMMON_PATH/qt/logging.cpp

COMMON_QT_HEADERS = \
    $$COMMON_PATH/qt/qt_interop.h \
    $$COMMON_PATH/qt/latest_image_slot.h \
    $$COMMON_PATH/qt/logging.h \
    $$COMMON_PATH/qt/test_runtime.h

COMMON_QT_SOURCES = \
    $$COMMON_PATH/qt/qt_interop.cpp \
    $$COMMON_PATH/qt/latest_image_slot.cpp \
    $$COMMON_PATH/qt/logging.cpp











isEmpty(QTCOMPAT_PRI_INCLUDED) {
    QTCOMPAT_PRI_INCLUDED = 1

    lessThan(QT_MAJOR_VERSION, 6) {
        lessThan(QT_MINOR_VERSION, 12) {
            win32-msvc* {
                QMAKE_CXXFLAGS += /std:c++17
            } else {
                QMAKE_CXXFLAGS += -std=gnu++17
            }
        }
    }
}





!isEmpty(LAB_WITH_OPENCV) {
    isEmpty(OPENCV_PRI_INCLUDED) {
        OPENCV_PRI_INCLUDED = 1

        OPENCV_MODULE_LIBS = \
            -lopencv_core \
            -lopencv_imgproc \
            -lopencv_imgcodecs \
            -lopencv_highgui \
            -lopencv_videoio \
            -lopencv_calib3d \
            -lopencv_features2d \
            -lopencv_objdetect \
            -lopencv_photo

        win32 {
            error("This project supports Linux only.")
        }
        macx {
            error("This project supports Linux only.")
        }
        !win32:!unix {
            error("This project supports Linux only.")
        }

        unix:!macx {
            OPENCV_READY =
            OPENCV_ROOT = $$clean_path($$COMMON_PATH/opencv)
            exists($$OPENCV_ROOT/include/opencv2/core.hpp):exists($$OPENCV_ROOT/lib) {
                OPENCV_INCLUDE_DIR = $$OPENCV_ROOT/include
                OPENCV_LIB_DIR = $$OPENCV_ROOT/lib
                INCLUDEPATH += $$OPENCV_INCLUDE_DIR
                LIBS += -L$$quote($$OPENCV_LIB_DIR) $$OPENCV_MODULE_LIBS
                OPENCV_READY = 1
            }

            # 内置库不可用时才读取环境变量；工程本身不保存任何绝对路径。
            isEmpty(OPENCV_READY) {
                OPENCV_ROOT = $$(OPENCV_ROOT)
                isEmpty(OPENCV_ROOT): OPENCV_ROOT = $$(OpenCV_ROOT)
                isEmpty(OPENCV_ROOT): OPENCV_ROOT = $$(OpenCV_DIR)
                OPENCV_INCLUDE_DIR = $$(OPENCV_INCLUDE_DIR)
                OPENCV_LIB_DIR = $$(OPENCV_LIB_DIR)
            }
            isEmpty(OPENCV_READY):!isEmpty(OPENCV_ROOT) {
                exists($$OPENCV_ROOT/include/opencv4/opencv2/core.hpp) {
                    OPENCV_INCLUDE_DIR = $$OPENCV_ROOT/include/opencv4
                }
                exists($$OPENCV_ROOT/include/opencv2/core.hpp) {
                    OPENCV_INCLUDE_DIR = $$OPENCV_ROOT/include
                }
                exists($$OPENCV_ROOT/lib) {
                    OPENCV_LIB_DIR = $$OPENCV_ROOT/lib
                }
                exists($$OPENCV_ROOT/lib64) {
                    OPENCV_LIB_DIR = $$OPENCV_ROOT/lib64
                }
                !isEmpty(OPENCV_INCLUDE_DIR):!isEmpty(OPENCV_LIB_DIR) {
                    INCLUDEPATH += $$OPENCV_INCLUDE_DIR
                    LIBS += -L$$quote($$OPENCV_LIB_DIR) $$OPENCV_MODULE_LIBS
                    OPENCV_READY = 1
                }
            }
            isEmpty(OPENCV_READY) {
                !isEmpty(OPENCV_INCLUDE_DIR) {
                    !isEmpty(OPENCV_LIB_DIR) {
                        INCLUDEPATH += $$OPENCV_INCLUDE_DIR
                        LIBS += -L$$quote($$OPENCV_LIB_DIR) $$OPENCV_MODULE_LIBS
                        OPENCV_READY = 1
                    }
                }
            }
            isEmpty(OPENCV_READY):packagesExist(opencv4) {
                CONFIG += link_pkgconfig
                PKGCONFIG += opencv4
                OPENCV_READY = 1
            }
            isEmpty(OPENCV_READY):exists(/usr/include/opencv4/opencv2/core.hpp) {
                INCLUDEPATH += /usr/include/opencv4
                LIBS += $$OPENCV_MODULE_LIBS
                OPENCV_READY = 1
            }
            isEmpty(OPENCV_READY) {
                error("OpenCV was not found.")
            }
        }
    }
}
