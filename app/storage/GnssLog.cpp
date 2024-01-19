/**
 * @file SensorLog.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-10-09
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/storage/GnssLog.hpp"
#include "app/proto/nmea/types.h"
#include "app/storage/backup.hpp"
#include "app/storage/log_partition.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME gnsslog
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (2)
#endif
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Public

/**
 * @brief
 *
 * @param record_size
 */
void GnssLog::init()
{
    Log::init();
    const ringfs_flash_partition_t *partition = log_gnss_partition();
    Log::ring_fs_init(partition, sizeof(gnss_record_v1_t), &m_fs, nullptr);
}

/**
 * @brief
 *
 */
void GnssLog::save_to_backup()
{
    // bkup_save_sensor_fs(&m_fs);
}
//<<----------------------