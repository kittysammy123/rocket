cmake_minimum_required(VERSION 3.25)

project(rocket VERSION 1.0.0 DESCRIPTION "c++ rpc learn")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cc")


#设置可执行文件输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#设置库文件路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


#设置include路径
include_directories(${CMAKE_SOURCE_DIR}/include)


add_library(log  ${SRC_FILES})
target_link_libraries(log PUBLIC tinyxml2)
#target_include_directories(log PUBLIC ${CMAKE_SOURCE_DIR}/include)
# add_executable(log  ${SRC_LOG_FILE})

add_subdirectory(test)
