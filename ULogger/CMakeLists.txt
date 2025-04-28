
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MY_SRC_DIRS)

# 先编译为 OBJECT 库（不生成最终库文件）
add_library(UL_LOGGER_OBJECT OBJECT ${MY_SRC_DIRS})

# 从 OBJECT 库生成静态库
if(BUILD_STATIC_LIB)
    add_library(UL_Logger STATIC $<TARGET_OBJECTS:UL_LOGGER_OBJECT>)
    target_include_directories(UL_Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(UL_Logger PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
endif()

# 从 OBJECT 库生成动态库
if(BUILD_SHARED_LIB)
    add_compile_definitions(UL_Logger_SHARED PUBLIC -DDEXPORT) 
    target_include_directories(UL_Logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(UL_Logger PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    )
endif()


add_custom_command(
    TARGET UL_Logger POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/Bin
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    COMMENT "Copying dll"
)

message("UncleDon::Logger  Libraries output direction: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
