# cmake file for TryCatchDemo
# Date		:  2017-03-03
# Author  	:  kniefliu (kniefliu@163.com)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(TryCatchDemo ${SRC_Root})
add_dependencies(TryCatchDemo TryCatchLib)
target_link_libraries(TryCatchDemo TryCatchLib)
