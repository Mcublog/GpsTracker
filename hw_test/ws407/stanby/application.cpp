/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-23
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdint>

#include "app/application.h"
#include "app/gps/data_types.h"
#include "app/io/gpio/gpio.h"
#include "app/storage/GnssLog.hpp"
#include "app/system/system.h"
#include "app/utils/build_marks.h"
#include "app/utils/data_print.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "app/version.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME wkptst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    LOG_RAW_INFO("\n");
    LOG_INFO("Version: %s", FW_VERSION);
    LOG_INFO("Git hash: %s", bm_get_fw_hash());
    LOG_INFO("Build time: %s", bm_get_fw_build_timestamp());

    io_gpio_red_led(true);
    System *sys = isystem();
    if (sys == nullptr)
    {
        LOG_ERROR("sys is not ready");
        System::infitite_loop();
    }
    sys->what();
    sys->init();

    wakeup_cause_t cause = sys->get_wakeup_cause();
    dprint_wakeup_cause(&cause);
    sys_mode_t mode = sys->mode_get();

    LOG_INFO("%s: mode: %s", tu_print_current_time_full(), sys->mode_stringify(mode));

    while (1)
    {
        delay_ms(1000);
        sys->go_to_stanby();
    }
}