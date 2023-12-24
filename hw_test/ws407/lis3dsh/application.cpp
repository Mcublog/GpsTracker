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
#include "libs/lis3dsh/lis3dsh.h"
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
    uint8_t id = LIS3DSH_ReadID();
    LOG_INFO("id: 0x%02x", id);

    bool red = true;
    while (true)
    {
        io_gpio_red_led(red);
        red ^= true;
        delay_ms(250);
    }
}