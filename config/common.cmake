set(EMU_TARGET                  tracker_emu)
set(EMU_TARGET_PATH             ${PROJECT_SOURCE_DIR}/targets/desktop)

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

# # Removed unused files
# list(REMOVE_ITEM DESKTOP_IMPL_SOURCES ${TARGETS_DIR}/desktop/main.cpp)

# App sources
file(GLOB_RECURSE APP_DIR_SOURCES
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp
)

set(LIBS_COMMON_INCLUDES
    ${LIBS_DIR}/lwgps-2.2.0/lwgps/src/include
    ${LIBS_DIR}/Ring-Buffer-0.1.1
    ${LIBS_DIR}/cobs-c-0.5.0
    ${LIBS_DIR}/ringfs-0.2.0
)

file(GLOB LWGPS_LIB_SOURCES         ${PROJECT_SOURCE_DIR}/libs/lwgps-2.2.0/lwgps/src/lwgps/*.c)
file(GLOB RING_BUFFER_LIB_SOURCES   ${PROJECT_SOURCE_DIR}/libs/Ring-Buffer-0.1.1/ringbuffer.c)
file(GLOB COBRSR_LIB_SOURCES        ${PROJECT_SOURCE_DIR}/libs/cobs-c-0.5.0/cobsr.c)
file(GLOB RINGFS_LIB_SOURCES        ${PROJECT_SOURCE_DIR}/libs/ringfs-0.2.0/ringfs.c)

#
set(HAL_WS407_INCLUDE_DIRS
    ${RTT_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/targets/ws407/Core/Inc
    ${PROJECT_SOURCE_DIR}/targets/ws407/Drivers/STM32F4xx_HAL_Driver/Inc
    ${PROJECT_SOURCE_DIR}/targets/ws407/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${PROJECT_SOURCE_DIR}/targets/ws407/Drivers/CMSIS/Include
    ${PROJECT_SOURCE_DIR}/targets/ws407/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)

set(HAL_WS407_USB_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/targets/ws407/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${PROJECT_SOURCE_DIR}/targets/ws407/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${PROJECT_SOURCE_DIR}/targets/ws407/USB_DEVICE/App
    ${PROJECT_SOURCE_DIR}/targets/ws407/USB_DEVICE/Target
)

set(HW_LIBS_INCLUDES
    ${RTT_INCLUDE_DIRS}
    ${LIBS_COMMON_INCLUDES}
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/lis3dsh
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/adxl345
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/W25Q
)
#
file(GLOB HW_PLATFORM_SOURCES
    ${PROJECT_SOURCE_DIR}/platforms/hw/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/*.cpp
    ${PROJECT_SOURCE_DIR}/platforms/hw/implementation/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/implementation/*.cpp
    ${PROJECT_SOURCE_DIR}/platforms/hw/system/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/system/*.cpp
    ${PROJECT_SOURCE_DIR}/platforms/hw/stm32/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/stm32/*.cpp
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/lis3dsh/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/adxl345/ADXL345.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/W25Q/w25q.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/libs/W25Q/w25calculator.cpp
)
#
file(GLOB_RECURSE HW_SOURCES
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp
    ${HW_PLATFORM_SOURCES}
    ${LWGPS_LIB_SOURCES}
    ${RING_BUFFER_LIB_SOURCES}
    ${COBRSR_LIB_SOURCES}
    ${RTT_SOURCES}
    ${RINGFS_LIB_SOURCES}
)

file(GLOB_RECURSE WS407_SOURCES
    ${PROJECT_SOURCE_DIR}/targets/ws407/*.s
    ${PROJECT_SOURCE_DIR}/targets/ws407/*.c
    ${PROJECT_SOURCE_DIR}/targets/ws407/*.cpp
)
