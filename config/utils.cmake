function(LOG_INFO text)
    message("[INFO] " ${ARGV})
endfunction()

function(LOG_DEBUG text)
    message("[DEBUG] " ${ARGV})
endfunction()

function(LOG_WARN text)
    message("[WARN] " ${ARGV})
endfunction()

function(create_bootable_bin target postfix version)
    set(OUTPUT_FILENAME PiritF_${postfix}_${version})

    add_custom_command(TARGET ${target}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex   -I elf32-littlearm ${target} ${target}.hex
        COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -I elf32-littlearm ${target} ${target}.bin
    )

    set(OUTPUT_BIN_PATH ${PROJECT_SOURCE_DIR}/bin/${OUTPUT_FILENAME}.bin)
    set(END4_BIN_PATH   ${PROJECT_SOURCE_DIR}/scripts/end4.bin)

endfunction()

function(build_jlink_script target)
    add_custom_command(TARGET ${target}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
        ARGS ${target}.hex ${PROJECT_SOURCE_DIR}/build/output/${target}.hex)

    configure_file(${PROJECT_SOURCE_DIR}/config/burn.jlink.in
        ${CMAKE_CURRENT_BINARY_DIR}/${target}.jlink
        ESCAPE_QUOTES)
endfunction()

function(get_git_hash hash_value)
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${hash_value} ${GIT_HASH} PARENT_SCOPE)
    if(GIT_HASH STREQUAL "")
        set(GIT_HASH "unknown")
    endif()
endfunction()

function(set_build_mark output_dir hash_value timestamp)

    set(GIT_HASH ${hash_value})
    set(BUILD_TIMESTAMP ${timestamp})

    configure_file(build_marks.c.in
        ${output_dir}/build_marks_actual.c
        ESCAPE_QUOTES)

endfunction()