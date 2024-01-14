/**
 * @file nmea.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-01-14
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "app/utils/nmea.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME unmea
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief
 *
 * @param gnss
 * @param tz
 * @return time_t
 */
time_t nmea::parse_time(const lwgps_t *gnss, int8_t tz)
{
    struct tm time = {
        .tm_sec = gnss->seconds,               // seconds [0,59]
        .tm_min = gnss->minutes,               // min [0,59]
        .tm_hour = gnss->hours,                // hour [0,23]
        .tm_mday = gnss->date,                 // mday [1,31]
        .tm_mon = gnss->month - 1,             // month [0,11]
        .tm_year = (gnss->year + 2000) - 1900, // year (1900 == 0)
        .tm_wday = 0,                          // week day (sunday = 0) [0,6]
        .tm_yday = 0,                          // year day (1 january = 0) [0,365]
        .tm_isdst = 0,                         // flag Summer time (>0 - off)
    };

    time_t t = mktime(&time);
    t += (tz * 60 * 60);
    return t;
}