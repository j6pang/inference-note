cmake_minimum_required(VERSION 3.16)

project(inference_note CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glog REQUIRED)
find_package(GTest REQUIRED)

add_library(infer STATIC
  infer/source/base/alloc.cpp
  infer/source/base/alloc_cpu.cpp
  infer/source/base/buffer.cpp
)
target_include_directories(infer PUBLIC ${PROJECT_SOURCE_DIR}/infer/include)
target_link_libraries(infer PUBLIC glog::glog)

enable_testing()
add_subdirectory(test)
