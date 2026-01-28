cmake_minimum_required(VERSION 4.1)

project(cppilot)

if(EXISTS "${celeritas_common_dir}")
    file(COPY ${celeritas_common_dir}/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/src/common)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))

    set(BUILD_TYPE_DIRECTORY debug)

else ()

    set(BUILD_TYPE_DIRECTORY release)

endif ()


if (MSVC)

    add_compile_definitions(_WIN32_WINNT=0x0A00)


    set(PREFIX msvc_)
    set(SUFFIX _${BUILD_TYPE_DIRECTORY})

else ()

    # 编译选项和符号可见性
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden")

    # 共享库不允许未定义符号
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")

    else ()

        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")

    endif ()

    set(PREFIX mingw_)
    set(SUFFIX _${BUILD_TYPE_DIRECTORY})

endif ()

set(BOOST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/boost")
set(LLAMA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/${PREFIX}llama.cpp${SUFFIX}")

# 包含路径
include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        "${BOOST_DIR}"
        "${LLAMA_DIR}/include"
        # 如果依赖项在本地，添加它们的include路径
)

add_link_options("-fopenmp")


# 包含boost库
add_library(boost INTERFACE)

set(BOOST_LIB_DIR "${BOOST_DIR}/stage/lib/")

if (MSVC)

    link_directories(${BOOST_LIB_DIR}/)

else ()

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

        add_compile_definitions(BOOST_ALL_DYN_LINK)

    endif ()

    set(CMAKE_PREFIX_PATH "${BOOST_LIB_DIR}/cmake")

    find_package(Boost 1.88 REQUIRED CONFIG COMPONENTS
            filesystem
            timer
            date_time
            thread
            charconv
            regex
            log_setup
            log
            program_options
            url
            json
            unit_test_framework)

    target_link_libraries(boost INTERFACE
            Boost::filesystem
            Boost::timer
            Boost::date_time
            Boost::thread
            Boost::charconv
            Boost::regex
            Boost::log_setup
            Boost::log
            Boost::program_options
            Boost::url
            Boost::json
            Boost::unit_test_framework)

endif ()

# 包含llama库
add_library(llama_lib INTERFACE)

link_directories(${LLAMA_DIR}/lib)

if (MSVC)

    target_link_libraries(llama_lib INTERFACE
            llama
            ggml
            ggml-cpu
            ggml-base
            mtmd)

else ()

    target_link_libraries(llama_lib INTERFACE
            llama
            :ggml.a
            :ggml-cpu.a
            :ggml-base.a
            mtmd)

endif ()

add_subdirectory(cmake/common)
add_subdirectory(cmake/model)
add_subdirectory(cmake/generate)
add_subdirectory(cmake/tokenizer)

add_library(cppilot_lib INTERFACE)

target_link_libraries(cppilot_lib INTERFACE
        tokenizer
        generate
        model
        common
        llama_lib
        boost)

add_subdirectory(cmake/cppilot)


# 安装规则
install(TARGETS
        cppilot
        DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

set(APP_RUNTIME_RPATH "$ORIGIN/lib")

set_target_properties(
        cppilot
        PROPERTIES INSTALL_RPATH "${APP_RUNTIME_RPATH}")


install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/config/"
        DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}/config/"
        PATTERN "*.xml")

set(THIRD_PARTY_DLLS)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    # 定义库的最终安装目标目录
    set(APP_LIB_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}")

    file(GLOB _dlls
            "${LLAMA_DIR}/lib/*.dll")

else ()

    # 定义库的最终安装目标目录
    set(APP_LIB_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/${BUILD_TYPE_DIRECTORY}/lib")

    file(GLOB _dlls
            "${BOOST_DIR}/stage/lib/*.so*")

endif ()

list(APPEND THIRD_PARTY_DLLS ${_dlls})

install(FILES ${THIRD_PARTY_DLLS}
        DESTINATION ${APP_LIB_INSTALL_DIR}
        PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE)

enable_testing()
add_subdirectory(cmake/tests)
