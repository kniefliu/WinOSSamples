# cmake file for TryCatchLib
# Date		:  2018-07-03
# Author  	:  kniefliu (kniefliu@163.com)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_definitions(-DTRYCATCHLIB_EXPORTS)
add_library(TryCatchLib SHARED ${SRC_Root})

