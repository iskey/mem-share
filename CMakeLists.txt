cmake_minimum_required (VERSION 2.6)

#Project Name
project(mem-share)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

include_directories(${PROJECT_SOURCE_DIR}/src/include/)

add_subdirectory(src)
