include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/libs.cmake)

set(EMU_TARGET                 tracker_emu)

# Mocking sources
file(GLOB_RECURSE DESKTOP_IMPL_SOURCES
    ${TARGETS_DIR}/desktop/*.c
    ${TARGETS_DIR}/desktop/*.cpp
)


file(GLOB_RECURSE EMU_PLATFORM_SOURCES
    ${PLATFORMS_DIR}/emu/*.c
    ${PLATFORMS_DIR}/emu/*.cpp
)

# Add includes dir
set(EMU_INCLUDES
    ${DESKTOP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/targets/desktop
    ${lwgps_SOURCE_DIR}/lwgps/src/include
    ${ring_buffer_SOURCE_DIR}
    ${cobsc_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}
    ${APP_DIR}
    ${CONFIG_DIR}
)

# Add sources
file(GLOB_RECURSE EMU_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${PLATFORMS_DIR}/emu/*.c
    ${PLATFORMS_DIR}/emu/*.cpp
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp
    ${LWGPS_LIB_SOURCES}
    ${RING_BUFFER_LIB_SOURCES}
    ${COBRSR_LIB_SOURCES}
)
