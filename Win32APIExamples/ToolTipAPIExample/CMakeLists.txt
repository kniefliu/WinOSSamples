# cmake file for ToolTipAPIExample
# Date		:  2018-07-31
# Author  	:  kniefliu (kniefliu@163.com)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SRC_Resource "${CMAKE_CURRENT_SOURCE_DIR}/*.rc")
file(GLOB SRC_Resource_res "${CMAKE_CURRENT_SOURCE_DIR}/res/*.ico" "${CMAKE_CURRENT_SOURCE_DIR}/res/*.bmp")
source_group("res" FILES ${SRC_Resource_res})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(ToolTipAPIExample WIN32 ${SRC_Root} ${SRC_Resource} ${SRC_Resource_res})

