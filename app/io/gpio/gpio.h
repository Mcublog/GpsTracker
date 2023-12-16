#ifndef APP_IO_GPIO_H
#define APP_IO_GPIO_H

#include <stdbool.h>

typedef void(*acc_irq_callback_t)(void);

#ifdef __cplusplus
extern "C"
{
#endif

    void io_gpio_green_led(bool on);
    void io_gpio_red_led(bool on);

    void io_init_external_power_pin(void);
    bool io_read_external_power_pin(void);
    bool io_read_wakeup_pin(void);
    bool io_read_accel_irq_pin(void);

    void io_acc_irq_set_handler(acc_irq_callback_t handler);
    void io_acc_handler(void);

#ifdef __cplusplus
}
#endif

#endif // APP_IO_GPIO_H