
#ifndef W24CALCULATOR_H
#define W24CALCULATOR_H

#include <stddef.h>
#include <stdint.h>

#include "app/system/common.h"
#include "w25q.h"

// TODO: move it to config
#define NUMBER_SECTORS_IN_ONE_W25 (16 * 64)

#ifdef __cplusplus
extern "C" {
#endif

    uint32_t w25calc_raw_address_to_w25(uint32_t max_memory_chip, uint32_t raw_addr,
                                       w25q_address_t *w25_addr);
    uint32_t w25calc_chunk_calculate(uint32_t max_memory_chip, uint32_t *address,
                                     uint8_t *chunk, w25q_address_t *w25_addr,
                                     buffer_view_t *view, size_t *size);

#ifdef __cplusplus
}
#endif

#endif // W24CALCULATOR_H