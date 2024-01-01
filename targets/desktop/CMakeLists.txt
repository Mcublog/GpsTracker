include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/desktop.cmake)
include(${PROJECT_SOURCE_DIR}/config/libs.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

# Set defines
add_definitions(-DprojCOVERAGE_TEST=0)
add_definitions(-D_WINDOWS_)
add_definitions(-DDESKTOP)

# Add includes dir
include_directories(${DESKTOP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/targets/desktop
    ${lwgps_SOURCE_DIR}/lwgps/src/include
    ${ring_buffer_SOURCE_DIR}
    ${cobsc_SOURCE_DIR}
)

# Add sources
file(GLOB_RECURSE DESKTOP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${PLATFORMS_DIR}/emu/*.c
    ${PLATFORMS_DIR}/emu/*.cpp
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp
)

# Buid DESKTOP target
add_executable(${DESKTOP_TARGET}
    ${DESKTOP_SOURCES}
    ${LWGPS_LIB_SOURCES}
    ${RING_BUFFER_LIB_SOURCES}
    ${COBRSR_LIB_SOURCES}
)

# Include paths
target_include_directories(${DESKTOP_TARGET} PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${APP_DIR}
    ${CONFIG_DIR}
)

# # Add pthread
target_link_libraries(${DESKTOP_TARGET} pthread)