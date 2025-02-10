cmake_minimum_required(VERSION 3.16)

project(CommonLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)
set(OPENCV_DIR "C:/Qt/6.5.3/opencv/build")
include_directories(${OPENCV_DIR}/install/include)
add_library(CommonLib SHARED
  CommonLib_global.h
  commonlib.cpp
  commonlib.h
)

target_link_libraries(CommonLib PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(CommonLib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CommonLib PRIVATE ${OpenCV_LIBS})
target_compile_definitions(CommonLib PRIVATE COMMONLIB_LIBRARY)
