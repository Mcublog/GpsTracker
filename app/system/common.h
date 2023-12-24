
#ifndef APP_SYSTEM_COMMON_H
#define APP_SYSTEM_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef uint_fast8_t cerror_t;

    typedef struct
    {
        uint8_t *data;
        size_t size;
    } buffer_view_t;

#define MAKE_RESULT(type)                                                                \
    typedef struct                                                                       \
    {                                                                                    \
        cerror_t hasError;                                                                \
        type value;                                                                      \
    } result_##type

    MAKE_RESULT(uint8_t);
    MAKE_RESULT(uint16_t);
    MAKE_RESULT(float);
    MAKE_RESULT(int);

    typedef enum
    {
        APP_GPIO_IRQ_RISING = 0,
        APP_GPIO_IRQ_FALLING
    } app_t_irq_edge_t;

    typedef bool (*app_t_gpio_irq_handler_t)(app_t_irq_edge_t);

#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */

#define DIS_TIMER_INT_RX
#define EN_TIMER_INT_RX

    const char *cm_irq_edge_stringify(app_t_irq_edge_t edge);
    const char *cm_stringify_bool(bool value);

#ifdef __cplusplus
}
#endif

#endif // APP_SYSTEM_COMMON_H