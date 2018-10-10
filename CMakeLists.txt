cmake_minimum_required(VERSION 3.8)

project(UVCCap)
set(CMAKE_AUTOUIC_SEARCH_PATHS "forms")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()


find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
set(Qt5_DIR "/home/timmoth/Qt/5.11.2/gcc_64/lib/cmake")

set(HEADER_FILES
    "headers/mainwindow.h"
    "headers/uvc_thread.h"
    )

set(SRC_FILES
    "src/main.cpp"
    "src/mainwindow.cpp"
    "src/uvc_thread.cpp"
    )

set(UI_FILES
    "forms/mainwindow.ui"
    )

qt5_wrap_ui(UI_HEADER_FILES UI_FILES)
qt5_add_resources(QRCS resources.qrc)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(UVCCap 
                ${SRC_FILES} 
                ${HEADER_FILES} 
                ${UI_HEADER_FILES} 
                ${QRCS})


target_include_directories(UVCCap PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(UVCCap PRIVATE "headers" "forms")

target_include_directories(UVCCap PUBLIC 
    "/usr/local/include"
    "/usr/include"
    )

target_link_libraries(UVCCap 
    "/usr/local/lib/x86_64-linux-gnu/libuvc.so"
    "/usr/local/lib/libusb-1.0.so"
    "/usr/local/lib/libopencv_core.so"
    "/usr/local/lib/libopencv_highgui.so"
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)