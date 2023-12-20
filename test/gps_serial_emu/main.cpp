/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-20
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include "app/utils/delay.h"
#include "targets/desktop/impl/Sdev.hpp"
#include "targets/desktop/io_mock/io_mock.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME main
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

int main(void)
{
    iomock_init();
    LOG_INFO("GpsSerialEmuTest");
    SDevice m_gps_sdev = SDevice("/dev/ttyS13");

    while(1)
    {
        m_gps_sdev.read();
    }
}