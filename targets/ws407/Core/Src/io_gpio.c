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

static acc_irq_callback_t m_acc_handler = NULL;

/**
 * @brief
 *
 * @param on
 */
void io_gpio_red_led(bool on)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
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
    return HAL_GPIO_ReadPin(USB_ext_power_GPIO_Port, USB_ext_power_Pin) == GPIO_PIN_SET
               ? true
               : false;
}

/**
 * @brief Check accel IRQ pin
 *
 * @return true
 * @return false
 */
bool io_read_accel_irq_pin(void)
{
    return HAL_GPIO_ReadPin(ACC_INT_1_GPIO_Port, ACC_INT_1_Pin) == GPIO_PIN_RESET;
}


void io_acc_irq_set_handler(acc_irq_callback_t handler)
{
    m_acc_handler = handler;
}

void io_acc_handler(void)
{
    if (m_acc_handler)
        m_acc_handler();
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