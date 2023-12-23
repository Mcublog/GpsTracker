include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/mcu.cmake)
include(${PROJECT_SOURCE_DIR}/config/utils.cmake)
include(${PROJECT_SOURCE_DIR}/hw_test/ws407/hw_test.cmake)

set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
#
set(TEST_HW_TARGET "ws407_lis3dh_test")
#
# Removed unused files
list(REMOVE_ITEM SOURCES ${APP_DIR}/application.cpp)

# App
add_executable(${TEST_HW_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
    ${SOURCES}
)

# Include paths
target_include_directories(${TEST_HW_TARGET}
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/targets/f407common/
    ${CONFIG_DIR}
    ${LIBS_INCLUDES}
    ${HAL_WS407_INCLUDE_DIRS}
    ${HAL_WS407_USB_INCLUDE_DIRS})

# Project symbols
target_compile_definitions(${TEST_HW_TARGET} PRIVATE ${symbols_SYMB})

# Compiler options
target_compile_options(${TEST_HW_TARGET} PRIVATE ${ARM_NONE_EABI_FLAGS})

# Linker options
target_link_options(${TEST_HW_TARGET} PRIVATE ${STM32F407_LINKER_OPTION})

# # Convert output to hex and binary
add_custom_command(TARGET ${TEST_HW_TARGET}
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex -I elf32-littlearm ${TEST_HW_TARGET} ${TEST_HW_TARGET}.hex
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary -I elf32-littlearm ${TEST_HW_TARGET} ${TEST_HW_TARGET}.bin
)

build_jlink_script(${TEST_HW_TARGET})