/**
 * @file CobsParser.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Cobs parser
 * @version 0.1
 * @date 2023-12-17
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstring>

#include "app/proto/cobs/Parser.hpp"
#include "app/system/system.h"
#include "libs/cobs-c-0.5.0/cobsr.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME cpars
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Private function
#ifdef __cplusplus
extern "C"
{
#endif

    static uint32_t wrapper_irq_handler(ios_chunk_t *chunk)
    {
        return isystem()->cobs_parser()->irq_handler(chunk);
    }

#ifdef __cplusplus
}
#endif

#define INPUT_BUFFER_SIZE (256)
#define OUTPUT_BUFFER_SIZE (256)

static uint8_t m_input_buffer[INPUT_BUFFER_SIZE];
static uint8_t m_output_buffer[OUTPUT_BUFFER_SIZE];
//<<----------------------


//>>---------------------- Exported function

/**
 * @brief
 *
 * @param ctl -- control
 * @return true
 * @return false
 */
bool CobsParser::init(Serial *dev)
{
    m_ctl = {{m_input_buffer, INPUT_BUFFER_SIZE},
             {m_output_buffer, OUTPUT_BUFFER_SIZE},
             wrapper_irq_handler};
    m_sdev = dev;
    return m_sdev->Init(&m_ctl);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool CobsParser::is_message_received(void) const
{
    return m_msg_ready;
}

/**
 * @brief
 *
 * @param message
 * @return true
 * @return false
 */
ios_message_t *CobsParser::read_message(void)
{
    m_msg_ready = false;

    cobsr_decode_result result =
        cobsr_decode(m_ctl.out.data, m_ctl.out.limit, m_ctl.in.data, (--m_pos));
    m_pos = 0;
    if (result.status != COBSR_DECODE_OK)
    {
        LOG_ERROR("result decode: %d", result.status);
        return NULL;
    }
    LOG_INFO("rx: size: %d", ((ios_message_t *)m_ctl.out.data)->size);

    return (ios_message_t *)m_ctl.out.data;
}

/**
 * @brief
 *
 * @param limit
 * @return uint8_t*
 */
uint8_t *CobsParser::get_output_buffer(uint32_t *limit)
{
    *limit = m_ctl.in.limit;
    return (uint8_t *)m_ctl.in.data;
}

/**
 * @brief
 *
 * @param message
 * @return true
 * @return false
 */
bool CobsParser::write_message(uint8_t *message, uint32_t size)
{
    ios_message_t *msg = (ios_message_t *)m_ctl.in.data;
    msg->size = size;
    cobsr_encode_result result =
        cobsr_encode(m_ctl.out.data, m_ctl.out.limit, m_ctl.in.data, size);
    if (result.status != COBSR_ENCODE_OK)
    {
        LOG_ERROR("result encode: %d", result.status);
        return false;
    }
    if (size == (m_ctl.out.limit - 1))
    {
        LOG_ERROR("size to big");
        return false;
    }
    m_ctl.out.data[size++] = kBinaryEndChar;
    return m_sdev->Write(m_ctl.out.data, size);
}

/**
 * @brief
 *
 */
uint32_t CobsParser::irq_handler(ios_chunk_t *chunk)
{
    if (m_msg_ready)
        return m_pos;
    if (m_ctl.in.data == NULL || m_ctl.out.data == NULL)
    {
        LOG_ERROR("ios ctl not initialized");
        return m_pos;
    }
    if ((m_pos + chunk->size) >= m_ctl.in.limit)
    {
        LOG_ERROR("received buffer is overrun");
        m_pos = 0;
        return m_pos;
    }
    memcpy(&m_ctl.in.data[m_pos], chunk->data, chunk->size);
    m_pos += chunk->size;
    m_msg_ready = (m_ctl.in.data[m_pos - 1] == kBinaryEndChar);
    return m_pos;
}

//<<----------------------