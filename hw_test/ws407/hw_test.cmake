include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/libs.cmake)

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/targets/ws407/*.s
    ${PROJECT_SOURCE_DIR}/targets/ws407/*.c
    ${PROJECT_SOURCE_DIR}/targets/ws407/*.cpp
    ${PROJECT_SOURCE_DIR}/platforms/hw/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/*.cpp
    ${PROJECT_SOURCE_DIR}/libs/lis3dsh/*.c
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp
    ${LWGPS_LIB_SOURCES}
    ${RING_BUFFER_LIB_SOURCES}
    ${COBRSR_LIB_SOURCES}
    ${RTT_SOURCES}
    ${ADXL345_SOURCES}
)

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
    ${PROJECT_SOURCE_DIR}/targets/ws407/USB_DEVICE/Target)

set(LIBS_INCLUDES
    ${RTT_INCLUDE_DIRS}
    ${lwgps_SOURCE_DIR}/lwgps/src/include
    ${ring_buffer_SOURCE_DIR}
    ${cobsc_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/libs/lis3dsh
    ${PROJECT_SOURCE_DIR}/libs/adxl345)

# Removed unused files
list(REMOVE_ITEM SOURCES ${APP_DIR}/application.cpp)