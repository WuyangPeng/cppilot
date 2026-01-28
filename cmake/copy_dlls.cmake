macro(copy_third_party_dlls TARGET_NAME)

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

        file(GLOB LLAMA_DLL_FILES "${LLAMA_DIR}/bin/*.dll")

        foreach (llama_dll_files IN LISTS LLAMA_DLL_FILES)

            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E echo "--- Copying config file: ${llama_dll_files} to $<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${llama_dll_files}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    COMMENT "Copying dll files for ${llama_dll_files}")

        endforeach ()

    endif ()

endmacro()
