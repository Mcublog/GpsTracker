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

#include "app/interfaces/System.hpp"
#include "app/io/gpio/gpio.h"
#include "app/storage/backup.hpp"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME isys
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
//<<----------------------

/**
 * @brief Go to stanby mode
 *
 * @return true need to return from app
 * @return false
 */
bool System::go_to_stanby()
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
wakeup_cause_t System::get_wakeup_cause() const
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
 * if RTC is IDLE
 *
 * @return sys_mode_t
 */
sys_mode_t System::mode_get()
{
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
 * @param m
 * @return const char
 */
const char *System::mode_stringify(sys_mode_t m)
{
    if (m == IDLE)
        return "IDLE";
    // else if (m == AUTONOMOUS)
    return "AUTONOMOUS";
}

/**
 * @brief
 *
 * @param perf
 * @return
 */
void System::performance_set(sys_performance_t perf)
{

}

/**
 * @brief
 *
 * @return sys_performance_t
 */
sys_performance_t System::performance_get()
{
    LOG_INFO("performance_get: dummy");
    return sys_performance_t::NOMINAL;
}

/**
 * @brief
 *
 * @param p
 * @return const char*
 */
const char *System::performance_stringify(sys_performance_t p)
{
    if (p == LOW_POWER)
        return "LOW_POWER";
    // else if (p == NOMINAL)
    return "NOMINAL";
}

/**
 * @brief
 *
 * @return
 */
void System::reinit_peripheral()
{
    LOG_INFO("reinit_peripheral: dummy");
}

bool System::is_wake_up_event()
{
    return false;
}


void System::infitite_loop()
{
    LOG_ERROR("SYS INFINITE LOOP !!!");
    io_gpio_green_led(false);
    while (1)
    {
        //LOG_ERROR("wakeup: %d", is_wake_up_event());
        // LOG_ERROR("hclk: %s", performance_get() == sys_performance_t::LOW_POWER
        //                           ? "LOW_POWER"
        //                           : "NOMINAL");
        io_gpio_red_led(true);
        delay_ms(500);
        io_gpio_red_led(false);
    }
}