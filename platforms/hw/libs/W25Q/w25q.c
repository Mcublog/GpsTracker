/*
 * w25q.c
 *
 *  Created on: Sep 18, 2021
 *      Author: leocelente
 */

#include "w25q.h"
#include "app/system/common.h"

/**
 * Sends WRITE_ENABLE instruction
 */
static void enable_write(w25q_t flash)
{
    buffer_view_t _ = {.data = (uint8_t *)&W25Q_WR_EN, .size = 1};
    gpio_low(flash.dev.pin);
    spi_transmit(flash.dev, _);
    gpio_high(flash.dev.pin);
}

/**
 * Polls until we can write again
 */
static void wait_busy(w25q_t flash)
{
    uint8_t status = 0xFF;
    buffer_view_t data = {.data = &status, .size = 1};
    buffer_view_t instr = {.data = (uint8_t *)&W25Q_RD_STATUS1, .size = 1};
    gpio_low(flash.dev.pin);
    spi_transmit(flash.dev, instr);
    do
    {
        spi_receive(flash.dev, data);
    } while ((status & 0x01) == 0x01);
    gpio_high(flash.dev.pin);
}

static void wait_busy_with_busy_callback(w25q_t flash, void(*busy_callback)(void))
{
    uint8_t status = 0xFF;
    buffer_view_t data = {.data = &status, .size = 1};
    buffer_view_t instr = {.data = (uint8_t *)&W25Q_RD_STATUS1, .size = 1};
    gpio_low(flash.dev.pin);
    spi_transmit(flash.dev, instr);
    do
    {
        spi_receive(flash.dev, data);
        if (busy_callback)
            busy_callback();
    } while ((status & 0x01) == 0x01);
    gpio_high(flash.dev.pin);
}


cerror_t w25q_chip_erase(w25q_t flash)
{
    enable_write(flash);
    buffer_view_t _ = {.data = (uint8_t *)&W25Q_CHIP_ERASE, .size = 1};
    gpio_low(flash.dev.pin);
    cerror_t e = spi_transmit(flash.dev, _);
    gpio_high(flash.dev.pin);
    wait_busy(flash);
    return e;
}
/**
 * Gets last byte of model ID, indicating capacity
 */
static result_uint8_t get_capacity_id(w25q_t flash)
{
    uint8_t chipid[3] = {0};
    buffer_view_t buffer = {.data = chipid, .size = sizeof(chipid)};
    buffer_view_t _ = {.data = (uint8_t *)&W25Q_ID, .size = 1};
    gpio_low(flash.dev.pin);
    spi_transmit(flash.dev, _);
    cerror_t e = spi_receive(flash.dev, buffer);
    gpio_high(flash.dev.pin);
    result_uint8_t out = {.hasError = e, .value = chipid[2]};
    return out;
}

static uint32_t byte_address(w25q_address_t address)
{
    return (address.sector_index * W25Q_SECTOR_SIZE) +
           (address.page_index * W25Q_PAGE_SIZE) + address.offset_bytes;
}

static uint32_t page_count_from_block_count(uint32_t blocks)
{
    const int bytes_per_sector = W25Q_SECTOR_SIZE;
    const int sectors_per_block = 16;
    uint32_t bytes = (blocks * sectors_per_block * bytes_per_sector);
    uint32_t pages = bytes / W25Q_PAGE_SIZE;
    return pages;
}

static uint32_t page_count_from_id(uint8_t chipid)
{
    int blocks = 0;
    switch (chipid)
    {
    case 0x18: // 	w25q128
        blocks = 256;
        break;
    case 0x17: //	w25q64
        blocks = 128;
        break;
    case 0x16: //	w25q32
        blocks = 64;
        break;
    default:
    case 0x15: //	w25q16
        blocks = 32;
        break;
    }

    return page_count_from_block_count(blocks);
}

uint32_t page_count_from_model(w25q_model_e model)
{

    int blocks = 0;
    switch (model)
    {
    case W25Q128:
        blocks = 256;
        break;
    case W25Q64:
        blocks = 128;
        break;
    case W25Q32:
        blocks = 64;
        break;
    default: // deve dar warning pq todos estão no enum
    case W25Q16:
        blocks = 32;
        break;
    }

    return page_count_from_block_count(blocks);
}

cerror_t w25q_init(w25q_t *flash, bool check_capacity)
{
    gpio_high(flash->dev.pin);

    result_uint8_t res = get_capacity_id(*flash);
    if (res.hasError)
    {
        return res.hasError;
    }

    // signs of bad connection
    if (res.value == 0xFF || res.value == 0x00)
    {
        return 1;
    }

    uint32_t real_count = page_count_from_id(res.value);
    if (real_count != flash->page_count)
    {
        if (check_capacity)
        {
            return 1;
        }
        // override count from chip
        flash->page_count = real_count;
    }

    uint8_t status_default[] = {W25Q_WR_STATUS1, 0x00};
    buffer_view_t status_reg = {.data = status_default, .size = sizeof(status_default)};

    gpio_low(flash->dev.pin);
    cerror_t e = spi_transmit(flash->dev, status_reg);
    gpio_high(flash->dev.pin);

    wait_busy(*flash); // just in case
    return e;
}

cerror_t w25q_page_write(w25q_t flash, buffer_view_t tx_data, w25q_address_t address)
{
    if ((address.offset_bytes + tx_data.size > W25Q_PAGE_SIZE) || (tx_data.size == 0))
    {
        tx_data.size = W25Q_PAGE_SIZE - address.offset_bytes;
    }

    uint32_t byte_addr = byte_address(address);
    uint8_t addr_vec[3] = {byte_addr >> 16, byte_addr >> 8, byte_addr};
    buffer_view_t addr = {.data = addr_vec, .size = sizeof(addr_vec)};

    buffer_view_t instr = {.data = (uint8_t *)&W25Q_WRITE, .size = 1};

    cerror_t error = 0;

    wait_busy(flash);
    enable_write(flash);
    gpio_low(flash.dev.pin);
    error |= spi_transmit(flash.dev, instr);
    error |= spi_transmit(flash.dev, addr);
    error |= spi_transmit(flash.dev, tx_data);
    gpio_high(flash.dev.pin);
    wait_busy(flash);

    return error;
}

cerror_t w25q_page_read(w25q_t flash, buffer_view_t rx_data, w25q_address_t address)
{
    if ((rx_data.size > W25Q_PAGE_SIZE) || (rx_data.size == 0))
    {
        rx_data.size = W25Q_PAGE_SIZE;
    }

    uint32_t byte_addr = byte_address(address);
    uint8_t addr_vec[4] = {byte_addr >> 16, byte_addr >> 8, byte_addr, 0};
    buffer_view_t addr = {.data = addr_vec, .size = sizeof(addr_vec)};
    buffer_view_t instr = {.data = (uint8_t *)&W25Q_READ, .size = 1};
    cerror_t error = 0;

    gpio_low(flash.dev.pin);
    error |= spi_transmit(flash.dev, instr);
    error |= spi_transmit(flash.dev, addr);
    error |= spi_receive(flash.dev, rx_data);
    gpio_high(flash.dev.pin);
    return error;
}

cerror_t w25q_sector_erase(w25q_t flash, size_t sector_address)
{
    wait_busy(flash);
    uint32_t byte_addr = sector_address * W25Q_SECTOR_SIZE;
    uint8_t addr_vec[3] = {byte_addr >> 16, byte_addr >> 8, byte_addr};
    buffer_view_t addr = {.data = addr_vec, .size = sizeof(addr_vec)};

    buffer_view_t instr = {.data = (uint8_t *)&W25Q_ERASE, .size = 1};
    cerror_t error = 0;

    enable_write(flash);
    gpio_low(flash.dev.pin);
    error |= spi_transmit(flash.dev, instr);
    error |= spi_transmit(flash.dev, addr);
    gpio_high(flash.dev.pin);
    wait_busy(flash);
    return error;
}


cerror_t w25q_sector_erase_with_busy_callback(w25q_t flash, size_t sector_address, void(*busy_callback)(void))
{
    wait_busy(flash);
    uint32_t byte_addr = sector_address * W25Q_SECTOR_SIZE;
    uint8_t addr_vec[3] = {byte_addr >> 16, byte_addr >> 8, byte_addr};
    buffer_view_t addr = {.data = addr_vec, .size = sizeof(addr_vec)};

    buffer_view_t instr = {.data = (uint8_t *)&W25Q_ERASE, .size = 1};
    cerror_t error = 0;

    enable_write(flash);
    gpio_low(flash.dev.pin);
    error |= spi_transmit(flash.dev, instr);
    error |= spi_transmit(flash.dev, addr);
    gpio_high(flash.dev.pin);
    wait_busy_with_busy_callback(flash, busy_callback);
    return error;
}


cerror_t w25q_go_to_power_down(w25q_t flash)
{
    wait_busy(flash);

    buffer_view_t instr = {.data = (uint8_t *)&W25Q_POWER_DOWN, .size = 1};
    cerror_t error = 0;

    gpio_low(flash.dev.pin);
    error |= spi_transmit(flash.dev, instr);
    gpio_high(flash.dev.pin);
    return error;
}


bool w25q_is_busy(w25q_t flash)
{
    uint8_t status = 0xFF;
    buffer_view_t data = {.data = &status, .size = 1};
    buffer_view_t instr = {.data = (uint8_t *)&W25Q_RD_STATUS1, .size = 1};
    gpio_low(flash.dev.pin);
    spi_transmit(flash.dev, instr);
    spi_receive(flash.dev, data);
    gpio_high(flash.dev.pin);
    return (status & 0x01) == 0x01;
    // return false;
}

cerror_t w25q_wrp_disable(w25q_t flash)
{
    wait_busy(flash);

    uint8_t instr_buffer[2] = {W25Q_WR_STATUS1, 0};
    buffer_view_t instr = {.data = instr_buffer, .size = 2};
    cerror_t error = 0;

    enable_write(flash);
    gpio_low(flash.dev.pin);
    error |= spi_transmit(flash.dev, instr);
    gpio_high(flash.dev.pin);
    wait_busy(flash);
    return error;
}