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
#include "app/utils/delay.h"
#include "app/io/gpio/gpio.h"
#include "libs/lis3dh/lis3dh_driver.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME acctst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    uint8_t val = 0;
    status_t s = LIS3DH_GetWHO_AM_I(&val);
    LOG_INFO("status: 0x%02x value: 0x%02x", s, val);

    bool red = true;
    while (true)
    {
        io_gpio_red_led(red);
        red ^= true;
        delay_ms(250);
    }
}