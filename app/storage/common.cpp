/**
 * @file common.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-01-19
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "app/storage/common.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME scom
#define LOG_MODULE_LEVEL (2)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Read config from storage or use default
 *
 * @return config_t
 */
config_t config()
{
    config_t cfg = {};
    if (config_load(&cfg) == CONFIG_ERROR)
    {
        LOG_INFO("config is empty, using defalult");
        cfg = config_get_default();
    }
    return cfg;
}