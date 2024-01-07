/**
 * @file agpio.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Abstract GPIO implemetation
 * @version 0.1
 * @date 2023-08-19
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */

#include "app/io/gpio/agpio.h"
#include "main.h"

void gpio_low(gpio_pin_t pin)
{
    HAL_GPIO_WritePin((GPIO_TypeDef*)pin.port, pin.pin, GPIO_PIN_RESET);
}

void gpio_high(gpio_pin_t pin)
{
    HAL_GPIO_WritePin((GPIO_TypeDef*)pin.port, pin.pin, GPIO_PIN_SET);
}

void gpio_toggle(gpio_pin_t pin)
{
    HAL_GPIO_TogglePin((GPIO_TypeDef*)pin.port, pin.pin);
}