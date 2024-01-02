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
#include "platforms/emu/impl/Sdev.hpp"
#include "platforms/emu/io_mock/io_mock.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

int main(void)
{
    LOG_INFO("GpsSerialEmuTest");

    while(1)
    {
    }
}