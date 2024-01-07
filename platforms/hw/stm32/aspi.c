/**
 * @file aspi.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Abstarct SPI implementation
 * @version 0.1
 * @date 2023-08-19
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */

#include "app/io/serial/aspi.h"
#include "main.h"

static const uint32_t TIMEOUT = 10;

cerror_t spi_transceive(spi_device_t device, buffer_view_t rx_buffer,
                       buffer_view_t tx_buffer)
{

    if (tx_buffer.size != rx_buffer.size)
    {
        return 1;
    }
    cerror_t error =
        HAL_SPI_TransmitReceive((SPI_HandleTypeDef *)device.spi, tx_buffer.data,
                                rx_buffer.data, tx_buffer.size, TIMEOUT);
    return error;
}

cerror_t spi_receive(spi_device_t device, buffer_view_t buffer_view)
{
    cerror_t error = HAL_SPI_Receive((SPI_HandleTypeDef *)device.spi, buffer_view.data,
                                    buffer_view.size, TIMEOUT);
    return error;
}

cerror_t spi_transmit(spi_device_t device, buffer_view_t buffer_view)
{
    cerror_t error = HAL_SPI_Transmit((SPI_HandleTypeDef *)device.spi, buffer_view.data,
                                     buffer_view.size, TIMEOUT);
    return error;
}
