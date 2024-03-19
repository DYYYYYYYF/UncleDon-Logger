project(Logger)
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_HOST_WIN32})
    message("Current environment: Win32\n")
    message("Current build type: ${CMAKE_BUILD_TYPE}\n")

    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/Windows/release)
    else()
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/Windows/${CMAKE_BUILD_TYPE})
    endif()
endif()

if (${CMAKE_HOST_APPLE})
    message("Current environment: Apple MacOS\n")

    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/MacOS/release)
    else()
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/MacOS/${CMAKE_BUILD_TYPE})
    endif()
    
endif()

message(${CMAKE_BUILD_TYPE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MY_SRC_DIRS)

add_library(Logger ${MY_SRC_DIRS})
# add_executable(Logger main.cpp ${MY_SRC_DIRS})

target_include_directories(Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)