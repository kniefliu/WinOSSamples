# CMake root file for Win32APIExample related apps
# Date		:  2018-03-30
# Author  	:  kniefliu (kniefliu@163.com)
#
# 	CMake files for Win32 API Example
# 	file for each app is under the related sub-dir
#

cmake_minimum_required(VERSION 2.8)

project(Win32APIExample)

message(STATUS, "CMake project files for Win32APIExample")

# _MBCS or (UNICODE _UNICODE)
add_definitions(-D_UNICODE -DUNICODE)

# add each CMake file
add_subdirectory(MSAAAPIExample)
