/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-20
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdint>

#include "app/system/system.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "platforms/emu/io_mock/io_mock.h"
#include "app/system/WorkingWdt.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

static void m_alarm_a_handler(void);
static const tu_alarm_t m_alarms[TU_ALARM_LAST] = {{TU_ALARM_0, m_alarm_a_handler}};
//<<----------------------

//>>---------------------- Local function definition
void m_alarm_a_handler(void)
{
    LOG_INFO("alarm: %s", tu_print_current_time_only());
    tu_reset_alarm();
}
//<<----------------------

int main(void)
{
    iomock_init();
    tu_init();

    WorkingWdt wwdt = WorkingWdt();

    for (uint32_t i = 0; i < 10; i++)
        wwdt.event_getting();

    if (wwdt.is_treshold())
    {
        LOG_INFO("is_treshold == True");
    }
    while (1)
    {
        delay_ms(1);
    }
}