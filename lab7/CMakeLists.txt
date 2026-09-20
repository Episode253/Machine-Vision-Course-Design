cmake_minimum_required(VERSION 3.10)
project(lab7_tensorRT CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs videoio highgui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../yunet_trt_cpp
                 ${CMAKE_CURRENT_BINARY_DIR}/yunet_trt_cpp)

add_executable(lab7
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/runtime/worker_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/runtime/pipeline.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/runtime/frame_source.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/qt/qt_interop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/qt/latest_image_slot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/qt/logging.cpp
    detector_worker.cpp
    main.cpp
    mainwindow.cpp)

target_include_directories(lab7 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../yunet_trt_cpp/include
    ${OpenCV_INCLUDE_DIRS})

target_link_libraries(lab7 PRIVATE
    yunet_trt
    ${OpenCV_LIBS}
    Qt5::Widgets)