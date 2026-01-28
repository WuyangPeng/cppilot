file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/cppilot/*.cpp")

add_executable(cppilot ${SRC_DIR_LIST})

target_link_libraries(cppilot PRIVATE cppilot_lib)