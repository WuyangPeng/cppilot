file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/tokenizer/*.cpp")

add_library(tokenizer ${SRC_DIR_LIST})