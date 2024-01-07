/**
 * @file log.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-10-09
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/storage/Log.hpp"
#include "app/storage/backup.hpp"
#include "app/utils/delay.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME log
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Private
[[maybe_unused]] static bool validate_fs(const ringfs_t *fs)
{
    return (fs->object_size == 0 || fs->version == 0 || fs->slots_per_sector == 0) ==
           false;
}

void Log::ring_fs_init(const ringfs_flash_partition_t *partition,
                       const uint32_t chunk_size, ringfs_t *fs,
                       backup_load_t load_from_backup)
{
    // if (bkup_is_active())
    if (false) // NOTE: disable saving to backup
    {
        LOG_INFO("restore from backup");
        if (load_from_backup)
            load_from_backup(fs);
        fs->flash = partition;
        if (validate_fs(fs))
            return;
        LOG_ERROR("fs is not valid, remount");
    }
    // NOTE: chunk_size Must be multiple 256
    ringfs_init(fs, partition, 1, chunk_size);

    /* Scan and/or format before any data operations. */
    LOG_INFO("# scanning for filesystem...");
    LOG_INFO("fs: v %d obj: %d", fs->version, fs->object_size);
    int e = ringfs_scan(fs);
    if (e == 0)
    {
        LOG_INFO("# found existing filesystem, usage: %d/%d", ringfs_count_estimate(fs),
                 ringfs_capacity(fs));
    }
    else
    {
        LOG_ERROR("# no valid filesystem found, formatting.");
        ringfs_format(fs);
    }
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Log::is_idle(void)
{
    return log_partition_is_idle();
}

/**
 * @brief
 *
 */
void Log::idle_waiting(void)
{
    while (Log::is_idle() == false)
    {
        LOG_WARNING("is busy");
        delay_ms(1);
    }
}

//<<----------------------

//>>---------------------- Public
void Log::init()
{
    log_partition_init();
    Log::idle_waiting();
}

/**
 * @brief
 *
 * @param busy_callback
 */
void Log::set_long_busy_callback(busy_callback_t busy_callback)
{
    log_partition_set_busy_callback(busy_callback);
}

/**
 * @brief
 *
 * @param record
 */
void Log::append(const void *record)
{
    int e = ringfs_append(&m_fs, record);
    Log::idle_waiting();
    if (e != 0)
        LOG_ERROR("append: %d", e);
    LOG_DEBUG("usage: %d/%d", ringfs_count_estimate(&m_fs), ringfs_capacity(&m_fs));
}

/**
 * @brief
 *
 * @param record
 */
int Log::pop(void *record)
{
    int e = ringfs_fetch(&m_fs, record);
    Log::idle_waiting();
    if (e != 0)
        LOG_ERROR("fetch: %d", e);
    return e;
}

/**
 * @brief
 *
 */
void Log::discard(void)
{
    int e = ringfs_discard(&m_fs);
    Log::idle_waiting();
    if (e != 0)
        LOG_ERROR("discard: %d", e);
}

void Log::format(void)
{
    int e = ringfs_format(&m_fs);
    Log::idle_waiting();
    if (e != 0)
        LOG_ERROR("ringfs_format: %d", e);
}

/**
 * @brief
 *
 * @param current
 * @param capacity
 */
void Log::usage(uint32_t *current, uint32_t *capacity)
{
    *current = ringfs_count_estimate(&m_fs);
    *capacity = ringfs_capacity(&m_fs);
}

/**
 * @brief
 *
 */
void Log::rewing(void)
{
    int e = ringfs_rewind(&m_fs);
    Log::idle_waiting();
    if (e != 0)
        LOG_ERROR("rewind: %d", e);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Log::is_last_record(void)
{
    uint32_t current = ringfs_count_estimate(&m_fs);
    uint32_t capacity = ringfs_capacity(&m_fs);
    return current == (capacity - 1);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool Log::is_empty()
{
    return (ringfs_count_estimate(&m_fs) == 0);
}
//<<----------------------