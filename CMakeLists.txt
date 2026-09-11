cmake_minimum_required(VERSION 3.16)

project(inference_note CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glog REQUIRED)
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(test)
