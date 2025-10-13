# 设置默认输出路径
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Lib)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Lib)
endif()

# 头文件及资源
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MY_SRC_DIRS)

# 先编译为 OBJECT 库（不生成最终库文件）
add_library(UL_LOGGER_OBJECT OBJECT ${MY_SRC_DIRS})
target_compile_definitions(UL_LOGGER_OBJECT PRIVATE UNCLED_LOGGER_BUILD_SHARED_LIB UNCLED_LOGGER_EXPORT)

if(UL_BUILD_SHARED_LIB)
    # 生成动态库
    add_library(UL_Logger_SHARED SHARED $<TARGET_OBJECTS:UL_LOGGER_OBJECT>)
    target_include_directories(UL_Logger_SHARED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    # 设置输出目录和文件名
    set_target_properties(UL_Logger_SHARED PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}  # DLL
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}  # Linux .so
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}  # Import.lib
        OUTPUT_NAME "UL_Logger"          
    )
        
    # 创建统一的接口别名
    message("UncleDon::Logger: Building shared library as default")
    add_library(UL::Logger ALIAS UL_Logger_SHARED)
else()
    # 生成静态库
    add_library(UL_Logger_STATIC STATIC $<TARGET_OBJECTS:UL_LOGGER_OBJECT>)
    target_include_directories(UL_Logger_STATIC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    set_target_properties(UL_Logger_STATIC PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        OUTPUT_NAME "UL_Logger_static"
    )

    # 创建统一的接口别名
    message("UncleDon::Logger: Building static library as default")
    add_library(UL::Logger ALIAS UL_Logger_STATIC)
endif()

message("UncleDon::Logger Runtime output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("UncleDon::Logger Library output: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message("UncleDon::Logger Archive output: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")