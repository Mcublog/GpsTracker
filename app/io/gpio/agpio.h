#ifndef AGPIO_H
#define AGPIO_H

#include <stdint.h>

typedef struct {
  void *port;
  uint16_t pin;
} gpio_pin_t;

#ifdef __cplusplus
extern "C" {
#endif

    void gpio_low(gpio_pin_t pin);
    void gpio_high(gpio_pin_t pin);
    void gpio_toggle(gpio_pin_t pin);

#ifdef __cplusplus
}
#endif

#endif // AGPIO_H