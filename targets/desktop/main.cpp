/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdio>
#include <cstring>

#include "app/application.h"
#include "app/system/system.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "platforms/emu/io_mock/io_mock.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME main
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

int main(void)
{
    iomock_init();
    tu_init();
    LOG_INFO("Emu started...");
    while (1)
    {
        application();
        LOG_INFO("power down: %s", tu_print_current_time_only());
        while (isystem()->get_wakeup_cause().d32 == 0)
        {
            delay_ms(1);
        }
        LOG_INFO("========================");
    }
}
//<<----------------------
