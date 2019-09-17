cmake_minimum_required(VERSION 3.0.0)
project(self_compiler VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "-g -std=c++11")
add_subdirectory(src)
add_subdirectory(sample)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
