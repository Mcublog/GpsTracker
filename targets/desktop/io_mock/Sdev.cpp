/**
 * @file Sdev.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Serial device implemenation
 * @version 0.1
 * @date 2023-02-26
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

// TODO: remove dep: #include "app/system/system.hpp"
#include "app/utils/delay.h"
#include "targets/desktop/io_mock/Sdev.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME     sdev
#define LOG_MODULE_LEVEL    (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local declaration
#define INPUT_BUFFER_SIZE   (256)
#define OUTPUT_BUFFER_SIZE  (256)

static uint8_t m_input_buffer[INPUT_BUFFER_SIZE];
static uint8_t m_output_buffer[OUTPUT_BUFFER_SIZE];

static ios_ctl_t m_ctl = {
    {m_input_buffer,    INPUT_BUFFER_SIZE},
    {m_output_buffer,   OUTPUT_BUFFER_SIZE},
    nullptr
};

static int m_io_stream;
static pthread_t m_thread_id;
//<<----------------------

//>>---------------------- Local definitions
static void *m_receiving_data(void*)
{
    while (1)
    {
        uint8_t byte = 0;
        long size = read(m_io_stream, &byte, 1);
        if (size <= 0)
        {
            delay_ms(1);
            continue;
        }
        ios_chunk_t data = {&byte, 1};
        // TODO: remove dep: sys_get_serial_device()->IrqHandler(&data);
    }
}
//<<----------------------

//>>---------------------- Exported function
/**
 * @brief
 *
 * @param ctl -- control
 * @return true
 * @return false
 */
bool SDevice::Init(ios_ctl_t *ctl)
{
    Serial::Init(ctl);
    memcpy(ctl, &m_ctl, sizeof(m_ctl));

    LOG_INFO("Usage: %s", DEFAULT_SERIAL_PORT);
    m_io_stream = open(DEFAULT_SERIAL_PORT, O_RDWR | O_NONBLOCK);
    if (m_io_stream == (-1))
    {
        perror("open");
        exit(1);
    }
    pthread_create(&m_thread_id, NULL, m_receiving_data, NULL);
    return true;
}

/**
 * @brief Write to file
 *
 * @param data
 * @param size
 * @return true
 * @return false
 */
bool SDevice::Write(uint8_t *data, uint32_t size)
{
    LOG_INFO("Write: %d", size);
    if (m_io_stream == (-1))
    {
        LOG_ERROR("io stream");
        return false;
    }
    ssize_t writed_count = write(m_io_stream, data, size);
    return writed_count == size;
}
//<<----------------------