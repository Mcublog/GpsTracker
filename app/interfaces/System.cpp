// /**
//  * @file system.c
//  * @author Viacheslav (viacheslav@mcublog.ru)
//  * @brief Some system function
//  * @version 0.1
//  * @date 2023-02-18
//  *
//  * @copyright Viacheslav mcublog (c) 2023
//  *
//  */

/**
 * @file System.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <functional>

#include "app/common_type.h"
#include "app/interfaces/System.hpp"
#include "app/io/gpio/gpio.h"
#include "app/storage/backup.hpp"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME sys
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
#ifdef DESKTOP

#include "targets/desktop/io_mock/Sdev.hpp"
// #include "targets/desktop/mocks/AccEmu.hpp"
// #include "targets/desktop/mocks/SensorEmu.hpp"
// #include "targets/desktop/mocks/EmuVoltage.hpp"

static SDevice m_sdev = SDevice();
// static AccEmu m_acc = AccEmu();
// static SensorEmu m_sensor = SensorEmu();
// static EmuVoltage m_voltage = EmuVoltage();

#else

// #include "targets/stm32f407ve_common/src/AccLis3d.hpp"
// #include "targets/stm32f407ve_common/src/AdcVoltage.hpp"
// #include "targets/stm32f407ve_common/src/SensorBme280.hpp"
// #include "targets/stm32f407ve_common/src/UsbSerial.hpp"

// static USBSerial m_sdev = USBSerial();
// static AccLis3D m_acc = AccLis3D();
// static SensorBme280 m_sensor = SensorBme280();
// static AdcVoltage m_voltage = AdcVoltage();

#endif

// static SensorLog m_sensor_log = SensorLog();
// static AccLog m_acc_log = AccLog();
// static Scheduler m_scheduler = Scheduler();
// static Indication m_indication =
//     Indication((indication_mode_handler_t)indicataion_handler);
//<<----------------------

/**
 * @brief Go to stanby mode
 *
 * @return true need to return from app
 * @return false
 */
bool System::go_to_stanby(void)
{
    LOG_INFO("go to stanby mode");
    delay_ms(100);
    return true;
}

/**
 * @brief
 *
 * @return wakeup cause bits
 */
wakeup_cause_t System::get_wakeup_cause(void)
{
    wakeup_cause_t cause = {};
    if (io_read_accel_irq_pin())
    {
        LOG_DEBUG("WAKE_BY_ACCEL");
        cause.field.by_accel = 1;
    }
    if (io_read_external_power_pin())
    {
        LOG_DEBUG("WAKE_BY_EXTERNAL_POWER");
        cause.field.by_external_power = 1;
    }
    if (tu_is_rtc_alarm_cause())
    {
        LOG_DEBUG("WAKE_BY_RTC");
        cause.field.by_rtc = 1;
    }
    return cause;
}

/**
 * @brief Request system startup type
 * Currently determined by the clock,
 * if RTC is reset, it means cold start DELAYED_START
 *
 * @return sys_mode_t
 */
sys_mode_t System::mode_get(void)
{
    if (tu_get_current_time() < TIME_FOR_CHECK_SYNC)
    {
        // RTC is resets
        mode_set(sys_mode_t::DELAYED_START);
        return sys_mode_t::DELAYED_START;
    }
    uint32_t data = 0;
    bkup_load_sysmode(&data);
    return (sys_mode_t)data;
}

/**
 * @brief
 *
 * @param mode
 */
void System::mode_set(sys_mode_t mode)
{
    uint32_t data = (uint32_t)mode;
    bkup_save_sysmode(&data);
}

/**
 * @brief
 *
 * @return Serial*
 */
Serial *System::get_serial_device(void)
{
    return (Serial *)&m_sdev;
}

/**
 * @brief
 *
 * @param perf
 * @return
 */
void System::set_performance(sys_performance_t perf)
{

}

/**
 * @brief
 *
 * @return sys_performance_t
 */
sys_performance_t System::get_performance(void)
{
    LOG_INFO("get_performance: dummy");
    return sys_performance_t::NOMINAL;
}

/**
 * @brief
 *
 * @return
 */
void System::reinit_peripheral(void)
{
    LOG_INFO("reinit_peripheral: dummy");
}

bool System::is_wake_up_event(void)
{
    return false;
}


void System::infitite_loop(void)
{
    LOG_ERROR("SYS INFINITE LOOP !!!");
    while (1)
    {
        io_gpio_green_led(false);
        io_gpio_red_led(true);
        LOG_ERROR("wakeup: %d", is_wake_up_event());
        LOG_ERROR("hclk: %s", get_performance() == sys_performance_t::LOW_POWER
                                  ? "LOW_POWER"
                                  : "NOMINAL");
        delay_ms(500);
        io_gpio_red_led(false);
    }
}