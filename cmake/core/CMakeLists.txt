file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/core/*.cpp")

add_library(core ${SRC_DIR_LIST})