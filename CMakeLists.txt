Cmake_minimum_required(VERSION 3.17)
project(linux_demo)
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets PrintSupport)
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu;${CMAKE_PREFIX_PATH}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)  # 使用 C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})
file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*cpp)
file(GLOB IN_H ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB IN_HPP ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
SET(INCLUDE ${IN_H} ${IN_HPP})
set(DARKNET_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/lib/libdarknet.so)

add_executable(linux_demo mainwindow.ui ${SOURCE} ${INCLUDE})

# 链接库
target_link_libraries(linux_demo ${OpenCV_LIBS} ${DARKNET_LIBRARY} Qt5::Core Qt5::Widgets Qt5::PrintSupport)
# 定义 OPENCV 宏（如果 yolo_v2_class.hpp 需要）
target_compile_definitions(linux_demo PRIVATE OPENCV)
