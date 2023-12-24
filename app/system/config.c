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

#include "app/system/config.h"
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
    static const config_t DEFAULT = {.last = 0,
                                     // LIS3DH_ODR_400Hz
                                     .rpt_cfg.accel.freq = 0x07,
                                     // LIS3DH_FULLSCALE_2
                                     .rpt_cfg.accel.full_scale = 0, // in lis units
                                     .rpt_cfg.accel.acc_size = 100,     // acc_buf size
                                     .rpt_cfg.accel.pre_threshold = 50, // in lis units depend on full_scale
                                     .rpt_cfg.accel.alert_threshold = 80, // in lis units depend on full_scale
                                     // int C temp
                                     .rpt_cfg.tph_cfg.period_s = 10,
                                     .rpt_cfg.tph_cfg.temperature_threshold.min = -4550,
                                     .rpt_cfg.tph_cfg.temperature_threshold.max = 4550,
                                     // (%)
                                     .rpt_cfg.tph_cfg.humidity_threshold.min = 5,
                                     .rpt_cfg.tph_cfg.humidity_threshold.max = 99,
                                     // (mBar)
                                     .rpt_cfg.tph_cfg.pressure_threshold.min = 260,
                                     .rpt_cfg.tph_cfg.pressure_threshold.max = 1200,

                                     // Sun, 04 Oct 2020 06:20:58 GMT
                                     .rpt_cfg.time_to_start = 1601792458,
                                     // Sun, 04 Oct 2020 06:20:57 GMT
                                     .rpt_cfg.time_to_stop = 1601792458 + 1,

                                     .aoffst.x = 0,
                                     .aoffst.y = 0,
                                     .aoffst.z = 0,

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

/**
 * @brief
 *
 * @param config
 * @return true
 * @return false
 */
bool config_validate_times(config_t *config)
{
    if (config_load(config) == CONFIG_ERROR)
    {
        LOG_ERROR("config is FAILED");
        return false;
    }
    if (config->rpt_cfg.time_to_stop <= config->rpt_cfg.time_to_start)
    {
        LOG_ERROR("config->rpt_cfg.time_to_stop <= config->rpt_cfg.time_to_start");
        LOG_ERROR("config time to start is incorrect value");
        return false;
    }
    if (config->rpt_cfg.tph_cfg.period_s == 0)
    {
        LOG_ERROR("config->rpt_cfg.tph_cfg.period_s == 0");
        return false;
    }
    return true;
}
