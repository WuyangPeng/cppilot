file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/generate/*.cpp")

add_library(generate ${SRC_DIR_LIST})