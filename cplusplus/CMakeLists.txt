# CMake root file for cplusplus related
# Date		:  2018-06-23
# Author  	:  kniefliu (kniefliu@163.com)
#
# 	CMake files for cplusplus example
# 	file for each app is under the related sub-dir
#

cmake_minimum_required(VERSION 2.8)

project(cplusplus)

message(STATUS, "CMake project files for cplusplus")


# add each CMake file
add_subdirectory(src/HelloWorld)
add_subdirectory(src/HelloConsole)
