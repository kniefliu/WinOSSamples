# CMake root file for ProgrammingWindows related apps
# Date		:  2017-12-29
# Author  	:  kniefliu (kniefliu@163.com)
#
# 	CMake files for Programming Windows
# 	file for each app is under the related sub-dir
#

cmake_minimum_required(VERSION 2.8)

project(ProgrammingWindows)

message(STATUS, "CMake project files for ProgrammingWindows")

# _MBCS or (UNICODE _UNICODE)
add_definitions(-D_UNICODE -DUNICODE)

# add each CMake file
add_subdirectory(src/HelloWin)
add_subdirectory(src/SystemMetrics/Lib)
add_subdirectory(src/SystemMetrics/Num1)
