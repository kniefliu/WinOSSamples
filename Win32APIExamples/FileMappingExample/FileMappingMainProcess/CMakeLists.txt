# description :  Using Shared Memory in a Dynamic-Link Library
# Date		  :  2019-02-03
# Author  	  :  kniefliu (kniefliu@163.com)

file(GLOB SRC_Root "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(FileMappingMainProcess ${SRC_Root})
target_link_libraries(FileMappingMainProcess FileMappingDll)
add_dependencies(FileMappingMainProcess FileMappingChildProcess)

