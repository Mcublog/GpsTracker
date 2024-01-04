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

//>>---------------------- Locals
static const uint32_t kTimeScaleMs = 1;
//<<----------------------

int main(void)
{
    iomock_init();
    tu_init(kTimeScaleMs);

    WorkingWdt wwdt = WorkingWdt();

    wwdt.load();

    bool condition = wwdt.is_expired();
    if (condition)
    {
        LOG_ERROR("is_expired == True");
        return -1;
    }

    for (uint32_t i = 0; i < 10; i++)
    {
        wwdt.event_getting();
        condition = wwdt.is_treshold();
        if (condition)
        {
            LOG_INFO("on %d is_treshold == True", i);
            break;
        }
    }
    if (!condition)
    {
        LOG_ERROR("!condition");
        return -1;
    }

    for (uint32_t i = 0; i < 20; i++)
    {
        condition = wwdt.is_expired();
        if (condition)
        {
            LOG_INFO("on %d is_expired == True", i);
            wwdt.reset();
            wwdt.save();
            break;
        }
        LOG_INFO("%s: tick...", tu_print_current_time_only());
        delay_ms(1);
    }

    if (!condition)
    {
        LOG_ERROR("!condition");
        return -1;
    }

    wwdt.load();
    if (wwdt.is_treshold())
    {
        LOG_ERROR("is_treshold");
        return -1;
    }

    if (wwdt.is_expired() == false)
    {
        LOG_ERROR("!is_expired");
        return -1;
    }

    LOG_INFO("all ok");
    return 0;
}