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

set(LIBS_DIR                    ${PROJECT_SOURCE_DIR}/libs)
set(BOOTLOADER_SOURCE_DIR       ${PROJECT_SOURCE_DIR}/bootloader/sources)
set(PIRIT2F_DIR                 ${PROJECT_SOURCE_DIR}/pirit_2f)
set(PIRIT_SOURCE_DIR            ${PROJECT_SOURCE_DIR}/pirit_2f/src/sources)
set(PIRIT_PRINTER_SOURCE_DIR    ${PROJECT_SOURCE_DIR}/pirit_p/sources)
set(HW_DIR                      ${STM32F205_TARGET_PATH}/hw)
set(TEST_DIR                    ${PROJECT_SOURCE_DIR}/test)
set(TEST_HW_DIR                 ${STM32F205_TARGET_PATH}/test_hw)
set(STM32_PINOUT_DIR            ${STM32F205_TARGET_PATH}/pinout)
set(GD32_PINOUT_DIR             ${GD32F427_TARGET_PATH}/pinout)

set(HAL_INCLUDE_DIRS
    # Put here your include dirs, one in each line,
    # relative to CMakeLists.txt file location
    ${LIBS_DIR}/STM32F2xx_StdPeriph_Driver/inc
    ${LIBS_DIR}/CMSIS
)
# endregion

# region BOOTLODER
set(BOOT_USB_DIR ${LIBS_DIR}/usb)
set(BOOT_SYSTEM_DIRS
    ${STM32F205_TARGET_PATH}/fw/system
)

set(F205_BOOT_INCLUDE_DIRS
    ${HAL_INCLUDE_DIRS}
    ${BOOT_SYSTEM_DIRS}
    ${BOOT_USB_DIR}
    ${HW_DIR}
    ${APP_INCLUDES}
    ${BOOTLOADER_SOURCE_DIR}
    ${TARGETS_DIR}
)
# endregion

# region FW
set(FW_SYSTEM_DIR
    ${STM32F205_TARGET_PATH}/fw/system
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
