# cmake file for HelloConsole
# Date		:  2017-06-24
# Author  	:  kniefliu (kniefliu@163.com)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(HelloConsole ${SRC_Root} ${SRC_Resource} ${SRC_Resource_res})
