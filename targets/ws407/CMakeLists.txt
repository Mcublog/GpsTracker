include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/mcu.cmake)
include(${PROJECT_SOURCE_DIR}/config/utils.cmake)

set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

# Build lib
add_executable(${WS407_TARGET}
    ${HW_SOURCES}
    ${WS407_SOURCES}
)

# Include paths
target_include_directories(${WS407_TARGET}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}
    ${PLATFORMS_DIR}/hw/stm32
    ${CONFIG_DIR}
    ${HW_LIBS_INCLUDES}
    ${HAL_WS407_INCLUDE_DIRS}
    ${HAL_WS407_USB_INCLUDE_DIRS}
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