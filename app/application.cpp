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
#include "app/io/gpio/gpio.h"
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

    System *sys = isystem();
    if (sys == nullptr)
    {
        LOG_ERROR("sys is not ready");
        while (1)
        {
            /* code */
        }

    }
    sys->what();
    sys->init();

    wakeup_cause_t cause = sys->get_wakeup_cause();
    dprint_wakeup_cause(&cause);

    LOG_INFO("current time: %s", tu_print_current_time_full());
    if (cause.d32 == 0)
    {
        LOG_ERROR("EMPTY CAUSE, do nothing");
        if (sys->go_to_stanby())
            return;
        // sys_infitite_loop();
    }

    while (1)
    {
        if (cause.d32 == 0)
        {
            LOG_INFO("all work done -> go to sleep");
            if (sys->go_to_stanby())
                return;
        }

        if (cause.field.by_external_power)
        {
            Cobs::process();
        }
        cause = sys->get_wakeup_cause();
    }
}