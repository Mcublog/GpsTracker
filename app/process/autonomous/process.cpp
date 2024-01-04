/**
 * @file process.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "app/process/autonomous/process.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME auto
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
//<<----------------------

//>>---------------------- Exported
bool Autonomous::process(void)
{
    while(1)
    {
        // if experied => change state to idle => go to sleep
        // else if event_getting => continues
    }
}
//<<----------------------