cmake_minimum_required(VERSION 3.21)
project(UL_Logger)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(UL_BUILD_EXAMPLE_EXE "Build example project" ON)
option(UL_BUILD_SHARED_LIB "Build shared library instead of static" ON)

add_subdirectory(ULogger)

if(UL_BUILD_EXAMPLE_EXE)
    add_subdirectory(Example)
endif()