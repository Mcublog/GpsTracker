/**
 * @file system.CPP
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include "app/system/system.h"
#include "app/common_type.h"

__weak System &instance()
{
    return System::instance();
}