message("Enable build example")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Bin)
add_executable(Example "Example.cpp")
target_link_libraries(Example PRIVATE UL::Logger)

add_executable(MutiThreadExample "MutiThreadExample.cpp")
target_link_libraries(MutiThreadExample PRIVATE UL::Logger)

message("Example project execute direction: ${EXECUTABLE_OUTPUT_PATH}")
