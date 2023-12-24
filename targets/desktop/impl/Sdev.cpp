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
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "app/proto/cobs/wrapper.h"
#include "app/utils/delay.h"
#include "targets/desktop/impl/Sdev.hpp"
#include "app/system/system.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME     sdev
#define LOG_MODULE_LEVEL    (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local declaration
//<<----------------------

//>>---------------------- Local definitions
//<<----------------------

//>>---------------------- Exported function
SDevice::SDevice(const char *portname, void*(*read_thread)(void*))
{
    m_portname = portname;
    m_read_thread = read_thread;
}

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
    // memcpy(ctl, &m_ctl, sizeof(m_ctl));

    LOG_INFO("Usage: %s", m_portname);
    m_io_stream = open(m_portname, O_RDWR | O_NONBLOCK);
    if (m_io_stream == (-1))
    {
        perror("open");
        exit(1);
    }
    if (m_read_thread == nullptr)
    {
        LOG_ERROR("m_read_thread == nullptr");
        return false;
    }
    pthread_create(&m_thread_id, NULL, m_read_thread, NULL);
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