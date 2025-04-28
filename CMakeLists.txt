cmake_minimum_required(VERSION 3.10)
project(UL_Logger)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_SHARED_LIB)
    unset(BUILD_SHARED_LIB CACHE)
endif()

if(BUILD_STATIC_LIB)
    unset(BUILD_STATIC_LIB CACHE)
endif()

if(BUILD_EXAMPLE_EXE)
    unset(BUILD_EXAMPLE_EXE CACHE)
endif()

option(BUILD_SHARED_LIB "Build shared lib" OFF)
option(BUILD_STATIC_LIB "Build shared lib" ON)
option(BUILD_EXAMPLE_EXE "Build example project" ON)

if(${CMAKE_HOST_WIN32})
    message("UncleDon::Logger  Current environment: Win32")
    message("UncleDon::Logger  Current build type: ${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib/Windows/${CMAKE_BUILD_TYPE})
elseif (${CMAKE_HOST_APPLE})
    message("UncleDon::Logger  Current environment: Apple MacOS")
    message("UncleDon::Logger  Current build type: ${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/lib/Windows/${CMAKE_BUILD_TYPE})
endif()

if(BUILD_SHARED_LIB)
add_compile_definitions(UL_Logger_SHARED PUBLIC -DDEXPORT) 
add_compile_definitions(UL_Logger_SHARED PUBLIC -DBUILD_SHARED_LIB) 
elseif(BUILD_STATIC_LIB)
add_compile_definitions(UL_Logger_SHARED PUBLIC -DBUILD_STATIC_LIB) 
endif()

add_subdirectory(ULogger)

if(BUILD_EXAMPLE_EXE)
    add_subdirectory(Example)
endif()