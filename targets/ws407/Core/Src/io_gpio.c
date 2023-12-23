/**
 * @file io_gpio.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2022-10-27
 *
 * @copyright Viacheslav mcublog (c) 2022
 *
 */
#include "app/io/gpio/gpio.h"
#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME io_gpio
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief
 *
 * @param on
 */
void io_gpio_red_led(bool on)
{
    // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void io_gpio_green_led(bool on)
{
}

void io_init_external_power_pin(void)
{

}

/**
 * @brief Read external power pin, using power from USB
 *
 * @return true
 * @return false
 */
bool io_read_external_power_pin(void)
{
    // return !((bool)HAL_GPIO_ReadPin(JOYSTICK_A_GPIO_Port, JOYSTICK_A_Pin));
    return false;
}

/**
 * @brief Check accel IRQ pin
 *
 * @return true
 * @return false
 */
bool io_read_accel_irq_pin(void)
{
    // return !((bool)HAL_GPIO_ReadPin(JOYSTICK_B_GPIO_Port, JOYSTICK_B_Pin));
    return false;
}


void io_acc_irq_set_handler(acc_irq_callback_t handler)
{
    (void)handler;
}

void io_acc_handler(void)
{

}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool io_read_wakeup_pin(void)
{
    return false;
}