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
    m_ctl.irq_handler = wrapper_irq_handler;
    m_sdev = dev;
    return m_sdev->Init(&m_ctl);
}

/**
 * @brief
 *
 * @param chunk
 * @return uint32_t
 */
uint32_t GnssParser::irq_handler(ios_chunk_t *chunk)
{
}
//<<----------------------
