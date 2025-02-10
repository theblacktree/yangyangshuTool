cmake_minimum_required(VERSION 3.16)

project(yinyangshi_global VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)
# 查找OpenCV
set(OPENCV_DIR "C:/Qt/6.5.3/opencv/build")
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found!")
endif()

# 包含OpenCV头文件路径
include_directories(${OPENCV_DIR}/install/include)

# 设置commonlib头文件路径
include_directories(${CMAKE_SOURCE_DIR}/../CommonLib)

# 设置CommonLib库文件路径
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(yinyangshi_global
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        kun28opencvtask.h kun28opencvtask.cpp
        liaotupocvtask.h liaotupocvtask.cpp
        personaltupocvtask.h personaltupocvtask.cpp
        Resources.qrc
        yuhunopencvtask.h yuhunopencvtask.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET yinyangshi_global APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(yinyangshi_global SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(yinyangshi_global
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(yinyangshi_global PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(yinyangshi_global PRIVATE ${OpenCV_LIBS})
# 链接 CommonLib 库
target_link_libraries(yinyangshi_global PRIVATE CommonLib)

# 包含 CommonLib 的头文件路径
target_include_directories(yinyangshi_global PRIVATE ${CMAKE_SOURCE_DIR}/../CommonLib)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.yinyangshi_global)
endif()
set_target_properties(yinyangshi_global PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS yinyangshi_global
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(yinyangshi_global)
endif()
