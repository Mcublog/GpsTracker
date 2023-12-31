/**
 * @file process.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <ctime>

#include "app/process/autonomous/process.hpp"
#include "app/io/gpio/gpio.h"
#include "app/process/WorkingWdt.hpp"
#include "app/system/system.h"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME auto
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
static bool m_acc_irq = false;

static void acc_handler(void)
{
    m_acc_irq = true;
}
//<<----------------------

//>>---------------------- Exported
bool Autonomous::process(void)
{
    LOG_INFO("Autonomous::process: run");

    GnssParser *m_gnssp = isystem()->gnss_parser();
    WorkingWdt wwdt = WorkingWdt();
    wwdt.load();

    io_acc_irq_set_handler(acc_handler);

    bool run = true;
    time_t last_time = tu_get_current_time();
    static const uint32_t kTimeDiff = 5;

    while (run)
    {
        run = io_read_external_power_pin() || wwdt.is_expired() ? false : true;

        if (m_gnssp->is_message_received())
        {
            if ((tu_get_current_time() - last_time >= kTimeDiff) == false)
                continue;
            lwgps_t *gnss = m_gnssp->read_message();
            if (gnss->is_valid)
                last_time = tu_get_current_time();
            else
                continue;

            LOG_INFO("Valid status: %d: %s", gnss->is_valid,
                     tu_print_current_time_only());
            LOG_INFO("Time: %02d:%02d:%02d", gnss->hours, gnss->minutes, gnss->seconds);
            LOG_INFO("Latitude: %f degrees", gnss->latitude);
            LOG_INFO("Longitude: %f degrees", gnss->longitude);
            LOG_INFO("Altitude: %f meters", gnss->altitude);
            // Save data to memory

            // break;
        }

        if (m_acc_irq)
        {
            LOG_DEBUG("acc irq handle");
            m_acc_irq = false;
            wwdt.event_getting();
            // wwdt.print_state();
        }

        if (wwdt.is_treshold())
        {
            LOG_INFO("%s: continue working", tu_print_current_time_only());
            wwdt.reset();
        }
    }

    if (wwdt.is_expired())
    {
        LOG_INFO("wwdt is experied");
        wwdt.reset();
        wwdt.print_state();
        isystem()->mode_set(sys_mode_t::IDLE);
    }

    wwdt.save();
    return true;
}
//<<----------------------