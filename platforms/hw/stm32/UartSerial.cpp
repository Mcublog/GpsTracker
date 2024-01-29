/**
 * @file UartSerial.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-01-06
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "platforms/hw/stm32/UartSerial.hpp"

#include "main.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME uarts
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

bool UartSerial::init(ios_ctl_t *ctl)
{
    Serial::init(ctl);
    helth();
    return true;
}

bool UartSerial::helth()
{
    UART_HandleTypeDef *huart = USART_Get_Gnss_handle();

    bool err = LL_USART_IsActiveFlag_PE(huart->Instance) ||
               LL_USART_IsActiveFlag_PE(huart->Instance) ||
               LL_USART_IsActiveFlag_NE(huart->Instance) ||
               LL_USART_IsActiveFlag_ORE(huart->Instance) ||
               LL_USART_IsActiveFlag_IDLE(huart->Instance) ||
               LL_USART_IsActiveFlag_FE(huart->Instance);

    if (err)
    {
        LL_USART_DisableIT_RXNE(huart->Instance);

        LL_USART_ClearFlag_PE(huart->Instance);
        LL_USART_ClearFlag_FE(huart->Instance);
        LL_USART_ClearFlag_NE(huart->Instance);
        LL_USART_ClearFlag_ORE(huart->Instance);
        LL_USART_ClearFlag_IDLE(huart->Instance);
        LL_USART_ClearFlag_RXNE(huart->Instance);

        LL_USART_EnableIT_RXNE(huart->Instance);
    }

    return err == false;
};

bool UartSerial::write(uint8_t *data, uint32_t size)
{
    UART_HandleTypeDef *huart = USART_Get_Gnss_handle();
    return HAL_UART_Transmit(huart, data, size, 10) == HAL_OK;
}