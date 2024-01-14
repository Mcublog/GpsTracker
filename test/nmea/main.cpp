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

    lwgps_t gnss = {0};

    gnss.hours = 0x0b;
    gnss.minutes = 0x15;
    gnss.seconds = 0x1b;
    gnss.date = 0x0e;
    gnss.month = 0x01;
    gnss.year = 0x18;

    int8_t tz = 3;
    time_t t = nmea::parse_time(&gnss, tz);

    // 14 2024 11:21:27 GMT+0000
    if (t == 1705231287)
        return 0;

    // 14 2024 14:21:27 GMT+0000
    if (t == (1705242087))
        return 0;

    printf("t != (time_t)1705231287 || t != (time_t)(1705242087)\r\n");
    return -1;
}