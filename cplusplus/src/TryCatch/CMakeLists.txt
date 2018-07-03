# CMake root file for TryCatch related
# Date		:  2018-07-03
# Author  	:  kniefliu (kniefliu@163.com)
#
# 	CMake files for TryCatch example
# 	file for each app is under the related sub-dir
#

cmake_minimum_required(VERSION 2.8)

project(TryCatch)

message(STATUS, "CMake project files for TryCatch")


# add each CMake file
add_subdirectory(TryCatchLib)
add_subdirectory(TryCatchDemo)
