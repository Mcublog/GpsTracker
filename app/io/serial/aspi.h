
#ifndef ASPI_H
#define ASPI_H

#include "app/system/common.h"
#include "app/io/gpio/agpio.h"

typedef struct
{
    void *spi;
    gpio_pin_t pin;
} spi_device_t;

#ifdef __cplusplus
extern "C"
{
#endif

    cerror_t spi_transceive(spi_device_t device, buffer_view_t rx_buffer,
                           buffer_view_t tx_buffer);

    cerror_t spi_receive(spi_device_t device, buffer_view_t buffer_view);
    cerror_t spi_transmit(spi_device_t device, buffer_view_t buffer_view);

#ifdef __cplusplus
}
#endif

#endif // ASPI_H