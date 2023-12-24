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
#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME acctst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ACC_INT_1_Pin)
    {
        LOG_INFO("acc irq");
    }
}
/**
 * @brief Test application
 *
 */
void application(void)
{
    uint8_t id = LIS3DSH_ReadID();
    LOG_INFO("id: 0x%02x", id);

    LIS3DSH_DataRateCmd(LIS3DSH_DATARATE_50);

    LIS3DSH_InterruptConfigTypeDef LIS3DSH_InterruptStruct;
    /* Set LIS3DSH Interrupt configuration */
    LIS3DSH_InterruptStruct.Interrupt_Selection_Enable = LIS3DSH_INTERRUPT_1_ENABLE;
    LIS3DSH_InterruptStruct.Interrupt_Request = LIS3DSH_INTERRUPT_REQUEST_PULSED;
    LIS3DSH_InterruptStruct.Interrupt_Signal = LIS3DSH_INTERRUPT_SIGNAL_HIGH;
    LIS3DSH_InterruptStruct.State_Machine1_Enable = LIS3DSH_SM_ENABLE;
    LIS3DSH_InterruptStruct.State_Machine2_Enable = LIS3DSH_SM_ENABLE;
    LIS3DSH_InterruptStruct.State_Machine1_Interrupt = LIS3DSH_SM_INT1;
    LIS3DSH_InterruptStruct.State_Machine2_Interrupt = LIS3DSH_SM_INT1;

    LIS3DSH_InterruptConfig(&LIS3DSH_InterruptStruct);
    bool red = true;
    while (true)
    {
        io_gpio_red_led(red);
        red ^= true;
        delay_ms(250);
    }
}