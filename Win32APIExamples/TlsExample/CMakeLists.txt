# CMake root file for TlsAPIExample related apps
# Date		:  2019-02-03
# Author  	:  kniefliu (kniefliu@163.com)
#
# 	CMake files for Win32 API Example
# 	file for each app is under the related sub-dir
#

cmake_minimum_required(VERSION 2.8)

project(TlsAPIExample)

message(STATUS, "CMake project files for TlsAPIExample")

# _MBCS or (UNICODE _UNICODE)
add_definitions(-D_UNICODE -DUNICODE)

# add each CMake file
add_subdirectory(TlsDll)
add_subdirectory(TlsDemo)
