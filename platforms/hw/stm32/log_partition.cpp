/**
 * @file log_partition.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/storage/log_partition.hpp"

#include "app/io/gpio/agpio.h"
#include "app/io/serial/aspi.h"
#include "main.h"
#include "spi.h"
#include "platforms/hw/libs/W25Q/w25calculator.hpp"
#include "platforms/hw/libs/W25Q/w25q.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME logp
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (2)
#endif
#include "app/debug/log_libs.h"
//<<----------------------


//>>---------------------- Global variable
//<<----------------------

//>>---------------------- Locals
static constexpr uint32_t kMaxMemoryChips = 1;
static w25q_t m_flash_dev[kMaxMemoryChips];

static busy_callback_t m_busy_callback = NULL;

bool ll_init = false;
//<<----------------------

//>>---------------------- Private
static ssize_t m_read(uint32_t address, void *data, size_t size)
{
    LOG_DEBUG("m_read: %d size: %d", address, size);
    ssize_t res = size;
    w25q_address_t w25_addr;
    buffer_view_t view;
    uint32_t next_chunk_address = address;
    uint8_t *chunk = (uint8_t *)data;

    while (size)
    {
        uint32_t memory_idx = w25calc_chunk_calculate(
            kMaxMemoryChips, (uint32_t *)&next_chunk_address,
            &chunk[next_chunk_address - address], &w25_addr, &view, &size);
        cerror_t e = w25q_page_read(m_flash_dev[memory_idx], view, w25_addr);
        if (e != 0)
            return 0;
    }

    return res;
}

static ssize_t m_program(uint32_t address, const void *data, size_t size)
{
    LOG_DEBUG("m_program: %d size: %d", address, size);
    ssize_t res = size;
    w25q_address_t w25_addr;
    buffer_view_t view;
    uint32_t next_chunk_address = address;
    uint8_t *chunk = (uint8_t *)data;

    while (size)
    {
        uint32_t memory_idx = w25calc_chunk_calculate(
            kMaxMemoryChips, (uint32_t *)&next_chunk_address,
            &chunk[next_chunk_address - address], &w25_addr, &view, &size);
        cerror_t e = w25q_page_write(m_flash_dev[memory_idx], view, w25_addr);
        if (e != 0)
            return 0;
    }

    return res;
}

static int m_sector_erase(uint32_t address)
{
    LOG_DEBUG("m_sector_erase: %d", address);

    w25q_address_t w25_addr;
    uint32_t memory_idx =
        w25calc_raw_address_to_w25(kMaxMemoryChips, address, &w25_addr);
    LOG_DEBUG("memory idx: %d", memory_idx);
    w25q_sector_erase_with_busy_callback(m_flash_dev[memory_idx], w25_addr.sector_index,
                                         m_busy_callback);
    return 0;
}
// clang-format off
const static ringfs_flash_partition_t m_gnss_partition =
{
    /**< Sector size, in bytes. */
    .sector_size = W25Q_SECTOR_SIZE,
    /**< Partition offset, in sectors. */
    .sector_offset = 0,
    /**< Partition size, in sectors. */
    .sector_count = (NUMBER_SECTORS_IN_ONE_W25 * kMaxMemoryChips),

    .sector_erase = m_sector_erase,
    .program = m_program,
    .read = m_read
};
// clang-format on

static void w25_init(const gpio_pin_t *cs_pin, w25q_t *flash)
{
    SPI_HandleTypeDef *hspi = SPI_Get_Memory_Handle();
    spi_device_t spi_dev_flash = {.spi = hspi, .pin = *cs_pin};
    // TODO: move W25Q32 to config
    flash->page_count = page_count_from_model(W25Q32);
    flash->dev = spi_dev_flash;

    cerror_t err = 0;
    do
    {
        err = w25q_init(flash, true);
        if (err)
            LOG_ERROR("w25q_init: %d, try reinit", err);
    } while (err);
}

//<<----------------------

/**
 * @brief
 *
 */
static void init(void)
{
    gpio_pin_t flash_cs[] = {{
        .port = (void *)ACC_nCS0_GPIO_Port,
        .pin = ACC_nCS0_Pin,
    }};

    for (uint32_t i = 0; i < kMaxMemoryChips; i++)
    {
        w25_init(&flash_cs[i], &m_flash_dev[i]);
    }
}

/**
 * @brief
 *
 */
void log_partition_init(void)
{
    if (ll_init)
        return;
    init();
    ll_init = true;
}

/**
 * @brief
 *
 * @return ringfs_flash_partition_t
 */
const ringfs_flash_partition_t *log_gnss_partition(void)
{
    return &m_gnss_partition;
}

/**
 * @brief
 *
 * @param busy_callback
 */
void log_partition_set_busy_callback(busy_callback_t busy_callback)
{
    m_busy_callback = busy_callback;
}

bool log_partition_is_idle(void)
{
    bool busy = false;
    for (uint32_t i = 0; i < kMaxMemoryChips; i++)
    {
        busy |= w25q_is_busy(m_flash_dev[i]);
    }
    return busy == false;
}