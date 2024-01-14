/**
 * @file config.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-02-25
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "app/config/config.h"
#include "app/system/common.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME config
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local declaration
//<<----------------------

/**
 * @brief Return default config
 *
 * @return config_t
 */
config_t config_get_default(void)
{
    static const config_t DEFAULT = {.version = 1,
                                     .s.serial = 0xFFFFFFFF, // by default all 1
                                     // crc
                                     .crc32 = 0};

    return DEFAULT;
}

__weak config_error_t config_save(config_t *config)
{
    return CONFIG_OK;
}

/**
 * @brief Config loading, if does not exist
 * returned DEFAULT
 *
 * @param config
 * @return config_error_t
 */
__weak config_error_t config_load(config_t *config)
{
    return CONFIG_OK;
}

/**
 * @brief Erase config
 *
 * @return config_error_t
 */
__weak config_error_t config_erase(void)
{
    return CONFIG_OK;
}
