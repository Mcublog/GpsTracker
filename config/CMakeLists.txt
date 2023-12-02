configure_file("version.h.in"
    ${PROJECT_SOURCE_DIR}/app/version.h)


configure_file("fw_burn.jlink.in"
    ${PROJECT_SOURCE_DIR}/scripts/flash_burn.jlink)

get_git_hash(GIT_HASH)
string(TIMESTAMP TIMESTAMP_MARK)

message("Got git hash: " ${GIT_HASH})
message("Build timestamp: " ${TIMESTAMP_MARK})

set_build_mark(${PROJECT_SOURCE_DIR}/app/utils ${GIT_HASH} ${TIMESTAMP_MARK})

# set_build_mark(${BOOTLOADER_SOURCE_DIR}       ${GIT_HASH} ${TIMESTAMP_MARK})
# set_build_mark(${PIRIT_SOURCE_DIR}            ${GIT_HASH} ${TIMESTAMP_MARK})
# set_build_mark(${PIRIT_PRINTER_SOURCE_DIR}    ${GIT_HASH} ${TIMESTAMP_MARK})
# set_build_mark(${TEST_DIR}                    ${GIT_HASH} ${TIMESTAMP_MARK})
