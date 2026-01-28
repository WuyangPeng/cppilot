file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/model/*.cpp")

add_library(model ${SRC_DIR_LIST})