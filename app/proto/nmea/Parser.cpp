/**
 * @file GnssParser.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-29
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstring>

#include "app/proto/nmea/Parser.hpp"
#include "app/system/system.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME gnss
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Exported function
#ifdef __cplusplus
extern "C" {
#endif

static uint32_t wrapper_irq_handler(ios_chunk_t *chunk)
{
    return isystem()->gnss_parser()->irq_handler(chunk);
}

#ifdef __cplusplus
}
#endif

/**
 * @brief
 *
 * @param dev
 * @return true
 * @return false
 */
bool GnssParser::init(Serial *dev)
{
    reset();
    m_sdev = dev;
    m_ctl.irq_handler = wrapper_irq_handler;
    return m_sdev->init(&m_ctl);
}

/**
 * @brief
 *
 */
void GnssParser::reset()
{
    ring_buffer_init(&m_rb, (char *)m_ring_buffer_pool, kRingBufferSize);
    lwgps_init(&m_hgps);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool GnssParser::is_message_received(void)
{
    while(ring_buffer_is_empty(&m_rb) == false)
    {
        char data = 0;
        ring_buffer_dequeue(&m_rb, &data);
        uint8_t state = lwgps_process(&m_hgps, &data, 1);

        if (state && m_hgps.is_valid && m_hgps.sats_in_use)
        {
            std::memcpy(&m_hgps_ready, &m_hgps, sizeof(lwgps_t));
            lwgps_init(&m_hgps);
            return true;
        }
    }
    if (m_sdev->helth() == false)
        reset();
    return false;
}

/**
 * @brief
 *
 * @return lwgps_t*
 */
lwgps_t *GnssParser::read_message(void)
{
    return &m_hgps_ready;
}

/**
 * @brief
 *
 * @param chunk
 * @return uint32_t
 */
uint32_t GnssParser::irq_handler(ios_chunk_t *chunk)
{
    ring_buffer_queue_arr(&m_rb, (const char*)chunk->data, chunk->size);
    return chunk->size;
}
//<<----------------------
