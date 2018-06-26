# cmake file for WindowFinder
# Date		:  2018-06-26
# Author  	:  kniefliu (kniefliu@163.com)

cmake_minimum_required(VERSION 2.8)

project(WindowFinder)

message(STATUS, "CMake project files for WindowFinder")

# _MBCS or (UNICODE _UNICODE)
add_definitions(-D_MBCS)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SRC_Resource "${CMAKE_CURRENT_SOURCE_DIR}/*.rc")
file(GLOB SRC_Resource_res "${CMAKE_CURRENT_SOURCE_DIR}/*.bmp" "${CMAKE_CURRENT_SOURCE_DIR}/*.cur")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(WindowFinder WIN32 ${SRC_Root} ${SRC_Resource} ${SRC_Resource_res})


