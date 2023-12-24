# region MCU GLOBAL PROPERTY
set(MCU_TARGET               tracker_fw)
set(WS407_TARGET             tracker_ws407_fw)

# Put here your symbols (preprocessor defines), one in each line
# Encapsulate them with double quotes for safety purpose
set(symbols_SYMB
    $<$<CONFIG:DEBUG>:"DEBUG">
    $<$<CONFIG:RELASE>:"NDEBUG">
    "USE_HAL_DRIVER"
    "STM32F407xx"
    "MCU"
)

set_property(GLOBAL PROPERTY STM32_DEFINES_PROPERTY
    ${symbols_SYMB}
)

set(CPU_PARAMETERS -mthumb
    # This needs attention to properly set for used MCU
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

set(ARM_NONE_EABI_FLAGS
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    # -Wpedantic
    -Wno-unused-parameter
    -Wno-pointer-arith
    -fdata-sections
    -ffunction-sections
    -fdiagnostics-color=always
    -fno-common
    -fmessage-length=0
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti -Wno-volatile>
    # Full debug configuration
    $<$<CONFIG:DEBUG>: -Og -g3 -ggdb>
    # Release
    $<$<CONFIG:RELEASE>:-Os -s>
    # Release with DEBUG
    $<$<CONFIG:RelWithDebInfo>:-Os -g3 -ggdb>
)

# Set linker script
# Set linker script
set(STM32F407_LINKER_SCRIPT ${PROJECT_SOURCE_DIR}/targets/ws407/STM32F407VETx_FLASH.ld)

set(STM32F407_LINKER_OPTION
    -T${STM32F407_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -specs=nano.specs
    -specs=nosys.specs
    # STDIO float formatting support
    -u_printf_float
    -u_scanf_float
    -Wl,--start-group
    -fno-exceptions
    -fno-rtti
    -Wl,--end-group
    -Wl,--gc-sections # removed unused functions
    -Wl,--print-memory-usage
    -lm
)
# endregion