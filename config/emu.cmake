include(${PROJECT_SOURCE_DIR}/config/common.cmake)

# Mocking sources
file(GLOB_RECURSE DESKTOP_IMPL_SOURCES
    ${TARGETS_DIR}/desktop/*.c
    ${TARGETS_DIR}/desktop/*.cpp
)

file(GLOB_RECURSE EMU_PLATFORM_SOURCES
    ${PLATFORMS_DIR}/emu/*.c
    ${PLATFORMS_DIR}/emu/*.cpp
)

file(GLOB_RECURSE FLASHSIM_SOURCES
    ${PROJECT_SOURCE_DIR}/libs/ringfs-0.2.0/tests/flashsim.c
)

# Add includes dir
set(EMU_INCLUDES
    ${DESKTOP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/targets/desktop
    ${LIBS_COMMON_INCLUDES}
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
    ${RINGFS_LIB_SOURCES}
    ${FLASHSIM_SOURCES}
)
