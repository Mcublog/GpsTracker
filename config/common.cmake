set(DESKTOP_TARGET_PATH             ${PROJECT_SOURCE_DIR}/targets/desktop)

# Include utils
# include(${PROJECT_SOURCE_DIR}/config/utils.cmake)

# region COMMON
set(APP_DIR ${PROJECT_SOURCE_DIR}/app)
set(PLATFORMS_DIR ${PROJECT_SOURCE_DIR}/platforms)
set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
set(CONFIG_DIR ${APP_DIR}/config)

# Root directory of targets
set(TARGETS_DIR ${PROJECT_SOURCE_DIR}/targets)

set(APP_INCLUDES
    ${PROJECT_SOURCE_DIR}
    ${APP_DIR}/debug
    ${APP_DIR}/client
    ${APP_DIR}/storage
    ${APP_DIR}/tools
)
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

# Mocking sources
file(GLOB_RECURSE DESKTOP_IMPL_SOURCES
    ${TARGETS_DIR}/desktop/*.c
    ${TARGETS_DIR}/desktop/*.cpp
)

# Removed unused files
list(REMOVE_ITEM DESKTOP_IMPL_SOURCES ${TARGETS_DIR}/desktop/main.cpp)

# App sources
file(GLOB_RECURSE APP_DIR_SOURCES
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp
)

file(GLOB_RECURSE EMU_PLATFORM_SOURCES
    ${PLATFORMS_DIR}/emu/*.c
    ${PLATFORMS_DIR}/emu/*.cpp
)


file(GLOB ADXL345_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/adxl345/*.c
)
