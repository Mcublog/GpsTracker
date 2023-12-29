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

#include "app/gps/GnssParser.hpp"
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
    lwgps_init(&m_hgps);
    m_ctl.irq_handler = wrapper_irq_handler;
    m_sdev = dev;
    return m_sdev->Init(&m_ctl);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool GnssParser::is_message_received(void) const
{
    return m_msg_ready;
}

/**
 * @brief
 *
 * @return lwgps_t*
 */
lwgps_t *GnssParser::read_message(void)
{
    m_msg_ready = false;
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
    m_msg_ready = (bool)lwgps_process(&m_hgps, chunk->data, chunk->size);
    if (m_msg_ready)
        std::memcpy(&m_hgps_ready, &m_hgps, sizeof(lwgps_t));
    return chunk->size;
}
//<<----------------------
