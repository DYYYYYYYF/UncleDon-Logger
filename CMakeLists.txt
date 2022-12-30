project(Logger)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./include)
aux_source_directory(./src MY_SRC_DIRS)

add_library(Logger ${MY_SRC_DIRS})
#add_executable(Logger main.cpp ${MY_SRC_DIRS})
