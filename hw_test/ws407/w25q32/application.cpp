/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-23
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdint>
#include <cstring>

#include "app/application.h"
#include "app/interfaces/System.hpp"
#include "app/io/gpio/agpio.h"
#include "app/io/gpio/gpio.h"
#include "app/io/serial/aspi.h"
#include "app/proto/nmea/types.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "app/utils/timer_measurement.h"

#include "platforms/hw/libs/W25Q/w25calculator.hpp"
#include "platforms/hw/libs/W25Q/w25q.h"
#include "platforms/hw/stm32/dwt.h"


#include "main.h"
#include "spi.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME logtst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    LOG_INFO("w25q test");

    w25q_address_t addr = {.sector_index = 0, .page_index = 0, .offset_bytes = 0};
    w25q_t flash = {};
    gpio_pin_t flash_cs = {
        .port = (void *)ACC_nCS0_GPIO_Port,
        .pin = ACC_nCS0_Pin,
    };
    SPI_HandleTypeDef *hspi = SPI_Get_Memory_Handle();
    spi_device_t spi_dev_flash = {.spi = hspi, .pin = flash_cs};

    // TODO: move W25Q32 to config
    flash.page_count = page_count_from_model(W25Q32);
    flash.dev = spi_dev_flash;

    dwt_reset();
    cerror_t err = w25q_init(&flash, true);
    uint32_t ticks = dwt_read();
    LOG_INFO("w25q_init: %ld us", tm_recalc_ticks_to_us(ticks));

    uint8_t buffer[256] = {0};
    buffer_view_t vb = {.data = buffer, .size = sizeof(buffer)};

    dwt_reset();
    w25q_wrp_disable(flash);
    w25q_sector_erase(flash, 0);
    w25q_chip_erase(flash);
    ticks = dwt_read();
    LOG_INFO("w25q_sector_erase: %ld us", tm_recalc_ticks_to_us(ticks));


    dwt_reset();
    err = w25q_page_read(flash, vb, addr);
    ticks = dwt_read();
    LOG_INFO("err: w25q_page_read: %d: %ld us", err, tm_recalc_ticks_to_us(ticks));

    for (uint32_t i = 0; i < sizeof(buffer); i++)
        buffer[i] = i;

    dwt_reset();
    err = w25q_page_write(flash, vb, addr);
    ticks = dwt_read();
    LOG_INFO("err: w25q_page_write: %d: %ld us", err, tm_recalc_ticks_to_us(ticks));

    std::memset(buffer, 0x00, sizeof(buffer));

    dwt_reset();
    err = w25q_page_read(flash, vb, addr);
    ticks = dwt_read();
    LOG_INFO("err: w25q_page_read: %d: %ld us", err, tm_recalc_ticks_to_us(ticks));

    while (1)
    {
        delay_ms(1);
    }
}