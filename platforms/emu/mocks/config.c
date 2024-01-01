/**
 * @file config.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Config
 * @version 0.1
 * @date 2023-02-24
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "app/system/config.h"
#include "platforms/emu/filelist.h"
#include "platforms/emu/io_mock/io_mock.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME config
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local declaration
//<<----------------------

config_error_t config_save(config_t *config)
{
    iomock_write_data(CONFIG_FILE_NAME, config, sizeof(config_t));
    return CONFIG_OK;
}

/**
 * @brief Config loading, if does not exist
 * returned DEFAULT
 *
 * @param config
 * @return config_error_t
 */
config_error_t config_load(config_t *config)
{
    if (iomock_file_is_exist(CONFIG_FILE_NAME) == false)
    {
        LOG_ERROR("Actual config not found");
        config_t cfg = config_get_default();
        if (iomock_file_create(CONFIG_FILE_NAME, &cfg, sizeof(config_t),
                               true) == false)
            exit(1);
        return CONFIG_ERROR;
    }
    else
        iomock_read_data(CONFIG_FILE_NAME, config, sizeof(config_t));
    return CONFIG_OK;
}

/**
 * @brief Erase config
 *
 * @return config_error_t
 */
config_error_t config_erase(void)
{
    return iomock_file_remove(CONFIG_FILE_NAME) ? CONFIG_OK : CONFIG_ERROR;
}
