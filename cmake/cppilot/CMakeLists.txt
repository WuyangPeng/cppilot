file(GLOB_RECURSE SRC_DIR_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../../src/cppilot/*.cpp")

add_executable(cppilot ${SRC_DIR_LIST})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    target_link_libraries(cppilot PRIVATE cppilot_lib)

else ()

    target_link_libraries(cppilot PRIVATE cppilot_lib dl)

    set_target_properties(cppilot PROPERTIES LINK_FLAGS "-rdynamic")

endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    file(GLOB CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/*.xml")

    foreach (config_file IN LISTS CONFIG_FILES)
        add_custom_command(TARGET cppilot POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${config_file} to $<TARGET_FILE_DIR:cppilot>/config/"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:cppilot>/config"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${config_file}" "$<TARGET_FILE_DIR:cppilot>/config/"
                COMMENT "Copying config files for ${config_file}")
    endforeach ()

    file(GLOB SERVER_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../../config/cppilot/*.xml")

    foreach (service_config_file IN LISTS SERVER_CONFIG_FILES)
        add_custom_command(TARGET cppilot POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${service_config_file} to $<TARGET_FILE_DIR:cppilot>/"
                COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:cppilot>/config/cppilot/"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${service_config_file}" "$<TARGET_FILE_DIR:cppilot>/config/cppilot/"
                COMMENT "Copying config files for ${service_config_file}")
    endforeach ()

    include(${CMAKE_CURRENT_LIST_DIR}/../copy_dlls.cmake)
    copy_third_party_dlls(cppilot)

endif ()
