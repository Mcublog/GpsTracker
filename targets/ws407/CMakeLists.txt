include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/mcu.cmake)
include(${PROJECT_SOURCE_DIR}/config/utils.cmake)

set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.s
    ${PROJECT_SOURCE_DIR}/platforms/hw/*.c
    ${PROJECT_SOURCE_DIR}/platforms/hw/*.cpp
    ${APP_DIR}/*.c
    ${APP_DIR}/*.cpp)

set(HAL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
)

set(USB_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target)

# Build lib
add_executable(${WS407_TARGET}
    ${SOURCES}
    ${RTT_SOURCES}
    ${LWGPS_LIB_SOURCES}
    ${RING_BUFFER_LIB_SOURCES}
    ${COBRSR_LIB_SOURCES})

# Include paths
target_include_directories(${WS407_TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/platforms/stm32
    ${HAL_INCLUDE_DIRS}
    ${APP_DIRS}
    ${CONFIG_DIR}
    ${RTT_INCLUDE_DIRS}
    ${USB_INCLUDES}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/targets/desktop
    ${LIBS_COMMON_INCLUDES}
)

# Project symbols
target_compile_definitions(${WS407_TARGET} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${WS407_TARGET} PRIVATE ${ARM_NONE_EABI_FLAGS})

# Linker options
target_link_options(${WS407_TARGET} PRIVATE ${STM32F407_LINKER_OPTION})

# # Convert output to hex and binary
add_custom_command(TARGET ${WS407_TARGET}
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -I elf32-littlearm ${WS407_TARGET} ${WS407_TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -I elf32-littlearm ${WS407_TARGET} ${WS407_TARGET}.bin
)

build_jlink_script(${WS407_TARGET})