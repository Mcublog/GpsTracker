/**
 * @file application.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Application entry point
 * @version 0.1
 * @date 2022-10-27
 *
 * @copyright Viacheslav mcublog (c) 2022
 *
 */
#include <cstring>
#include <stdbool.h>
#include <string>

#include "app/application.h"
#include "app/config/config.h"
#include "app/io/gpio/gpio.h"
#include "app/process/WorkingWdt.hpp"
#include "app/process/autonomous/process.hpp"
#include "app/process/external_power/process.hpp"
#include "app/system/system.h"
#include "app/utils/build_marks.h"
#include "app/utils/data_print.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "app/version.h"
#include "libs/lwgps-2.2.0/lwgps/src/include/lwgps/lwgps.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Private
//<<----------------------

/**
 * @brief
 *
 */
void application(void)
{
    LOG_RAW_INFO("\n");
    LOG_INFO("Version: %s", FW_VERSION);
    LOG_INFO("Git hash: %s", bm_get_fw_hash());
    LOG_INFO("Build time: %s", bm_get_fw_build_timestamp());

    io_gpio_red_led(true);

    if (tu_get_current_time() < (time_t)TIME_FOR_CHECK_SYNC)
    {
        const time_t kDefualtTime = TIME_FOR_CHECK_SYNC;
        tu_set_time(&kDefualtTime);
    }

    System *sys = isystem();
    if (sys == nullptr)
    {
        LOG_ERROR("sys is not ready");
        System::infitite_loop();

    }
    sys->what();
    sys->init();

    wakeup_cause_t cause = sys->get_wakeup_cause();

    config_t cfg = {};
    if (config_load(&cfg) == CONFIG_ERROR)
    {
        LOG_INFO("config is empty, using defalult");
        cfg = config_get_default();
    }

    // Query the reason kТаitingCauseTimeS time while the reason is empty
    time_t current = tu_get_current_time();
    static const uint32_t kWaitingCauseTimeS = 2;
    while (cause.d32 == 0 && cfg.log.manual_mode == 0)
    {
        cause = sys->get_wakeup_cause();
        if (tu_get_current_time() - current <= kWaitingCauseTimeS)
            continue;
        if (sys->go_to_stanby())
            return;
    }

    WorkingWdt wwdt = WorkingWdt();
    wwdt.load();

    if (cfg.log.manual_mode)
    {
        LOG_INFO("manual mode: TRUE");
        wwdt.reset();
        wwdt.event_getting();
        sys->mode_set(sys_mode_t::NORMAL);
    }
    else
        dprint_wakeup_cause(&cause);

    if (wwdt.is_expired())
    {
        LOG_INFO("wwdt is expired: reset it");
        wwdt.reset();
        sys->mode_set(sys_mode_t::IDLE);
    }

    if (cause.field.by_accel)
        wwdt.event_getting();

    if (wwdt.is_treshold())
    {
        LOG_INFO("wwdt is treshold: TRUE or manual mode: TRUE");
        sys->mode_set(sys_mode_t::NORMAL);
    }

    wwdt.save();
    wwdt.print_state();

    sys_mode_t mode = sys->mode_get();
    LOG_INFO("%s: mode: %s", tu_print_current_time_full(), sys->mode_stringify(mode));

    while (1)
    {
        if (cause.d32 == 0 && cfg.log.manual_mode == 0)
        {
            LOG_INFO("all work done -> go to sleep");
            if (sys->go_to_stanby())
                return;
        }

        if (cause.field.by_external_power)
        {
            ExtPower::process();
            mode = sys->mode_get();
            wwdt.reset();
            wwdt.save();
        }

        if (mode == sys_mode_t::NORMAL)
        {
            Autonomous::process();
            mode = sys->mode_get();
        }

        cause = sys->get_wakeup_cause();
    }
}