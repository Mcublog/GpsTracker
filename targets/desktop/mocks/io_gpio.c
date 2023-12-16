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
#include "targets/desktop/io_mock/io_mock.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME io_gpio
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local variables
#define RED_LED_GPIO_FILE_NAME      "/tmp/io_led_gpio_red"
#define GREEN_LED_GPIO_FILE_NAME    "/tmp/io_led_gpio_green"
#define EXT_PWR_GPIO_FILE_NAME      "/tmp/io_ext_pwr_pin"
#define ACCEL_IRQ_GPIO_FILE_NAME    "/tmp/io_accel_irq_pin"
//<<----------------------

void io_gpio_green_led(bool on)
{
    if (on)
        LOG_INFO("gpio green: switch ON");
    else
        LOG_INFO("gpio green: switch OFF");
    iomock_write_bool(GREEN_LED_GPIO_FILE_NAME, on);
}

void io_gpio_red_led(bool on)
{
    if (on)
        LOG_INFO("gpio red: switch ON");
    else
        LOG_INFO("gpio red: switch OFF");
    iomock_write_bool(RED_LED_GPIO_FILE_NAME, on);
}

void io_init_external_power_pin(void)
{
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool io_read_external_power_pin(void)
{
    return iomock_read_bool(EXT_PWR_GPIO_FILE_NAME);
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

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool io_read_accel_irq_pin(void)
{
    return iomock_read_bool(ACCEL_IRQ_GPIO_FILE_NAME);
}

void io_acc_irq_set_handler(acc_irq_callback_t handler)
{
    (void)handler;
}

void io_acc_handler(void)
{

}