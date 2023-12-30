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
#include "libs/adxl345/ADXL345.h"
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static bool red = true;
    if (GPIO_Pin == ACC_INT_1_Pin)
    {
        data_ready = true;
        io_gpio_red_led(red);
        red ^= true;
        LOG_DEBUG("acc irq");
    }
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
    adxl345h.AddressI2C = 0;
    adxl345h.PlatformI2CInit = init;
    adxl345h.PlatformI2CDeInit = deinit;
    adxl345h.PlatformI2CSend = write;
    adxl345h.PlatformI2CReceive = read;

    ADXL345_SetAddressI2C(&adxl345h, 0);
    while (true)
    {
        ADXL345_Result_t res = ADXL345_CheckDeviceID(&adxl345h);
        LOG_INFO("result: %d", res);

        delay_ms(500);
    }
}