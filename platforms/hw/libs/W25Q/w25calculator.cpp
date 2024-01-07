/**
 * @file w25calculator.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "w25calculator.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME w25calc
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
//<<----------------------

uint32_t w25calc_raw_address_to_w25(uint32_t max_memory_chip, uint32_t raw_addr,
                                   w25q_address_t *w25_addr)
{
    w25_addr->sector_index = (raw_addr / W25Q_SECTOR_SIZE) % NUMBER_SECTORS_IN_ONE_W25;
    w25_addr->page_index = raw_addr % W25Q_SECTOR_SIZE / W25Q_PAGE_SIZE;
    w25_addr->offset_bytes = raw_addr % W25Q_PAGE_SIZE;

    uint32_t idx = raw_addr / (W25Q_SECTOR_SIZE * NUMBER_SECTORS_IN_ONE_W25);
    if (idx >= max_memory_chip)
        LOG_ERROR("idx >= max_memory_chip");
    return idx >= max_memory_chip ? 0 : idx;
}

uint32_t w25calc_chunk_calculate(uint32_t max_memory_chip, uint32_t *address,
                                 uint8_t *chunk, w25q_address_t *w25_addr,
                                 buffer_view_t *view, size_t *size)
{
    uint32_t memory_idx = w25calc_raw_address_to_w25(max_memory_chip, *address, w25_addr);

    ssize_t chunk_size =
        (*size / W25Q_PAGE_SIZE) != 0 ? W25Q_PAGE_SIZE : (*size % W25Q_PAGE_SIZE);
    if (w25_addr->offset_bytes && *size > W25Q_PAGE_SIZE)
    {
        chunk_size = W25Q_PAGE_SIZE - w25_addr->offset_bytes;
    }
    else if ((chunk_size + w25_addr->offset_bytes) > W25Q_PAGE_SIZE)
    {
        chunk_size = W25Q_PAGE_SIZE - w25_addr->offset_bytes;
    }
    LOG_DEBUG("memory idx: %d chunk size: %d", memory_idx, chunk_size);
    view->data = chunk;
    view->size = chunk_size;

    *address += chunk_size;
    *size -= chunk_size;
    return memory_idx;
}