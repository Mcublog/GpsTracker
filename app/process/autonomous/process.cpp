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

#include "app/config/config.h"
#include "app/io/gpio/gpio.h"
#include "app/process/WorkingWdt.hpp"
#include "app/process/autonomous/process.hpp"
#include "app/proto/nmea/types.h"
#include "app/storage/GnssLog.hpp"
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

    config_t cfg = {};
    if (config_load(&cfg) == CONFIG_ERROR)
    {
        LOG_INFO("config is empty, using defalult");
        cfg = config_get_default();
    }

    GnssLog log = GnssLog();
    log.init();
    log.set_long_busy_callback(NULL);

    GnssParser *m_gnssp = isystem()->gnss_parser();
    m_gnssp->reset();

    WorkingWdt wwdt = WorkingWdt();
    wwdt.load();

    io_acc_irq_set_handler(acc_handler);

    static const uint32_t kTimeDiff = cfg.log.event_diff_time_s;
    static const uint32_t kDebugLogDiff = 10;

    bool run = true;
    time_t last_time = tu_get_current_time() - kTimeDiff;
    time_t debug_log_mark = tu_get_current_time();

    while (run)
    {
        run = io_read_external_power_pin() || wwdt.is_expired() ? false : true;
        run = io_read_external_power_pin() || cfg.log.manual_mode == 0 ? false : true;

        if (cfg.log.manual_mode)
            wwdt.reset();

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
            gnss_record_t gpsdata = {};
            gpsdata.tm = tu_get_current_time();
            gpsdata.latitude = gnss->latitude;
            gpsdata.longitude = gnss->longitude;
            gpsdata.altitude = gnss->altitude;
            log.append(&gpsdata);
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
            wwdt.reset();
        }

        if (tu_get_current_time() - debug_log_mark >= kDebugLogDiff)
        {
            uint32_t current, capacity;
            log.usage(&current, &capacity);
            LOG_INFO("%s: still working: storage: %d/%d", tu_print_current_time_only(),
                     current, capacity);
            debug_log_mark = tu_get_current_time();
        }
    }

    if (wwdt.is_expired())
    {
        LOG_INFO("%s: wwdt is experied", tu_print_current_time_only());
        wwdt.reset();
        wwdt.print_state();
        isystem()->mode_set(sys_mode_t::IDLE);
    }

    wwdt.save();
    return true;
}
//<<----------------------