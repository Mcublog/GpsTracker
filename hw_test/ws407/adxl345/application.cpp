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
#include "app/io/gpio/gpio.h"
#include "app/utils/delay.h"
#include "libs/hw/adxl345/ADXL345.h"
#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME acctst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Private
#define ADXL345_TIMEOUT (100)

extern I2C_HandleTypeDef hi2c1;

static volatile bool data_ready = true;

void acc_handler(void)
{
    static bool red = true;

    data_ready = true;
    io_gpio_red_led(red);
    red ^= true;
    LOG_DEBUG("acc irq");
}

static int8_t init(void)
{
    return 0;
}

static int8_t deinit(void)
{
    return 0;
}

static int8_t write(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
    Address <<= 1;
    if (HAL_I2C_Master_Transmit(&hi2c1, Address, Data, DataLen, ADXL345_TIMEOUT))
        return -1;

    return 0;
}

static int8_t read(uint8_t Address, uint8_t *Data, uint8_t DataLen)
{
    Address <<= 1;
    if (HAL_I2C_Master_Receive(&hi2c1, Address, Data, DataLen, ADXL345_TIMEOUT))
        return -1;

    return 0;
}

static ADXL345_Handler_t adxl345h = {};

//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    io_acc_irq_set_handler(acc_handler);

    adxl345h.AddressI2C = 0;
    adxl345h.PlatformI2CInit = init;
    adxl345h.PlatformI2CDeInit = deinit;
    adxl345h.PlatformI2CSend = write;
    adxl345h.PlatformI2CReceive = read;

    ADXL345_Result_t res = ADXL345_Init(&adxl345h);

    res = ADXL345_Set_Rate(&adxl345h, ADXL345_RATE_25);

    ADXL345_PowerControl_t pwr_ctrl = {};
    pwr_ctrl.Measure = 1;
    res = ADXL345_Set_PowerControl(&adxl345h, &pwr_ctrl);

    ADXL345_ActivityInactivity_t actconfig = {};

    actconfig.ActivityThreshold = 100;
    actconfig.InactivityThreshold = 25;
    actconfig.InactivityTime = 10;

    actconfig.Control.InactivityEnableZ = 1; // Enable inactivity for Z-axis
    actconfig.Control.InactivityEnableY = 1; // Enable inactivity for Y-axis
    actconfig.Control.InactivityEnableX = 1; // Enable inactivity for X-axis
    actconfig.Control.InactivityCoupled = 1; // 0 => DC , 1 => AC
    actconfig.Control.ActivityEnableZ   = 1; // Enable activity for Z-axis
    actconfig.Control.ActivityEnableY   = 1; // Enable activity for Y-axis
    actconfig.Control.ActivityEnableX   = 1; // Enable activity for X-axis
    actconfig.Control.ActivityCoupled   = 1; // 0 => DC , 1 => AC

    res = ADXL345_Set_ActivityInactivity(&adxl345h, &actconfig);

    ADXL345_InterruptConfig_t config = {};
    config.Enable.Activity = 1;
    config.Map.Activity = 1;

    res = ADXL345_Set_InterruptConfig(&adxl345h, &config);
    while (true)
    {
        res = ADXL345_CheckDeviceID(&adxl345h);
        LOG_INFO("result: %d", res);

        delay_ms(500);
    }
}