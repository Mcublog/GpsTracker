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
#include "app/gps/GnssParser.hpp"
#include "app/io/gpio/gpio.h"
#include "app/system/system.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME gpstst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    LOG_INFO("GPS test");

    System *sys = isystem();
    sys->what();
    sys->init();

    GnssParser *m_gnssp = isystem()->gnss_parser();

    while (true)
    {
        if (m_gnssp->is_message_received())
        {
            lwgps_t *gnss = m_gnssp->read_message();
            LOG_INFO("Valid status: %d: %s", gnss->is_valid,
                     tu_print_current_time_only());
            LOG_INFO("Time: %02d:%02d:%02d", gnss->hours, gnss->minutes, gnss->seconds);
            LOG_INFO("Sat using: %d", gnss->sats_in_use);
            LOG_INFO("Latitude: %f degrees", gnss->latitude);
            LOG_INFO("Longitude: %f degrees", gnss->longitude);
            LOG_INFO("Altitude: %f meters", gnss->altitude);

            io_gpio_red_led(true);
            delay_ms(250);
            io_gpio_red_led(false);
        }
        else
            delay_ms(1);
    }
}