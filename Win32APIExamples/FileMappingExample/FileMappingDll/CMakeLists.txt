# description :  Using Shared Memory in a Dynamic-Link Library
# Date		  :  2019-02-03
# Author  	  :  kniefliu (kniefliu@163.com)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_library(FileMappingDll SHARED ${SRC_Root})

