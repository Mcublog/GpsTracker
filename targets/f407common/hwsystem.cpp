/**
 * @file esystem.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include "app/system/common.h"
#include "app/system/system.h"
#include "targets/f407common/SystemHw.hpp"

static SystemHW m_system = SystemHW();

System *isystem()
{
    // return (System *)&m_system;
    return nullptr;
}
