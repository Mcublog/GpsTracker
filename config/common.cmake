set(STM32F205_TARGET_PATH           ${PROJECT_SOURCE_DIR}/targets/stm32f205)
set(GD32F427_TARGET_PATH            ${PROJECT_SOURCE_DIR}/targets/gd32f427)
set(DESKTOP_TARGET_PATH             ${PROJECT_SOURCE_DIR}/targets/desktop)

# Include utils
# include(${PROJECT_SOURCE_DIR}/config/utils.cmake)

# region COMMON
set(APP_DIR ${PROJECT_SOURCE_DIR}/app)

# Root directory of targets
set(TARGETS_DIR ${PROJECT_SOURCE_DIR}/targets)

set(APP_INCLUDES
    ${PROJECT_SOURCE_DIR}
    ${APP_DIR}/debug
    ${APP_DIR}/client
    ${APP_DIR}/storage
    ${APP_DIR}/tools
)

file(GLOB_RECURSE APP_SOURCES   ${APP_DIR}/*.c)

# endregion

# region RTT
set(RTT_ENABLE true)
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(RTT_ENABLE false)
endif()

if(${RTT_ENABLE})
    # LOG_INFO("RTT is enabled")
    set(RTT_VERSION SEGGER_RTT_V762c)
    set(RTT_DIR ${LIBS_DIR}/${RTT_VERSION})
    set(RTT_INCLUDE_DIRS
        ${RTT_DIR}/Config
        ${RTT_DIR}/RTT)
    # Add sources
    file(GLOB_RECURSE RTT_SOURCES
        ${RTT_DIR}/RTT/*.c
        ${RTT_DIR}/RTT/*.S
        ${RTT_DIR}/Syscalls/SEGGER_RTT_Syscalls_GCC.c
    )
endif()
# endregion
