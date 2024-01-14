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

#include "app/utils/nmea.hpp"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
//<<----------------------

int main(void)
{
    LOG_INFO("NMEA unit");

    const lwgps_t gnss = {.hours = 0x0b,
                    .minutes = 0x15,
                    .seconds = 0x1b,
                    .date = 0x0e,
                    .month = 0x01,
                    .year = 0x18};

    time_t t = nmea::parse_time(&gnss, 3);

    if (t != 1705231287)
        return -1;

    LOG_INFO("%s", tu_print_time_full(&t));
    return 0;
}