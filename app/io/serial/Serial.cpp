/**
 * @file Serial.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/io/Serial.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME serial
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "log_libs.h"
//<<----------------------

/**
 * @brief
 *
 * @param chunk
 * @return uint32_t
 */
uint32_t Serial::IrqHandler(ios_chunk_t *chunk)
{
    if (m_irq_handler == nullptr)
    {
        LOG_ERROR("m_irq_handler == nullptr");
        return 0;
    }
    return m_irq_handler(chunk);
}