include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MY_SRC_DIRS)

# 先编译为 OBJECT 库（不生成最终库文件）
add_library(UL_LOGGER_OBJECT OBJECT ${MY_SRC_DIRS})
target_include_directories(UL_LOGGER_OBJECT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 生成动态库
add_library(UL_Logger_SHARED SHARED $<TARGET_OBJECTS:UL_LOGGER_OBJECT>)
target_include_directories(UL_Logger_SHARED PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(UL_Logger_SHARED PUBLIC UL_Logger_SHARED)
target_compile_definitions(UL_Logger_SHARED PRIVATE DEXPORT)

# 生成静态库
add_library(UL_Logger_STATIC STATIC $<TARGET_OBJECTS:UL_LOGGER_OBJECT>)
target_include_directories(UL_Logger_STATIC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 设置输出目录和文件名
set_target_properties(UL_Logger_SHARED PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    OUTPUT_NAME "UL_Logger"
)

set_target_properties(UL_Logger_STATIC PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    OUTPUT_NAME "UL_Logger"
)

# 创建统一的接口别名
add_library(UL::Logger_SHARED ALIAS UL_Logger_SHARED)
add_library(UL::Logger_STATIC ALIAS UL_Logger_STATIC)

# 根据BUILD_SHARED_LIB选择默认的UL::Logger指向
if(BUILD_SHARED_LIB)
    message("UncleDon::Logger: Building shared library as default")
    add_library(UL::Logger ALIAS UL_Logger_SHARED)
else()
    message("UncleDon::Logger: Building static library as default")
    add_library(UL::Logger ALIAS UL_Logger_STATIC)
endif()

# 复制DLL文件（仅Windows需要）
if(WIN32 AND BUILD_SHARED_LIB)
    add_custom_command(
        TARGET UL_Logger_SHARED POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:UL_Logger_SHARED>
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Copying shared library to runtime directory"
    )
endif()

message("UncleDon::Logger Libraries output directory: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
