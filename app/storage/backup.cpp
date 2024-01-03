/**
 * @file backup.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-09-20
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <cstring>

#include "app/storage/backup.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME bkup
#define LOG_MODULE_LEVEL (2)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
#define BACKPSRAM_SIZE (4 * 1024)

typedef struct
{
    uint32_t active_mark;
    uint32_t sys_mode;
    uint8_t event_cnt;
    time_t event_mark;
} bakup_map_t;

typedef union {
    bakup_map_t map;
    uint8_t array[sizeof(bakup_map_t)];
} bakup_map;


static bakup_map m_backup_map __attribute__((section(".bkpram")));

//<<----------------------

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool bkup_is_active(void)
{
    return m_backup_map.map.active_mark != 0;
}

/**
 * @brief
 *
 */
void bkup_set_active(void)
{
    m_backup_map.map.active_mark = 0xDEADBEAF;
}

/**
 * @brief
 *
 */
void bkup_reset_active(void)
{
    m_backup_map.map.active_mark = 0;
}

/**
 * @brief
 *
 * @param rb
 */
void bkup_save_sysmode(uint32_t *mode)
{
    LOG_INFO("bkup_save_sysmode");
    std::memcpy(&m_backup_map.map.sys_mode, mode, sizeof(uint32_t));
}

/**
 * @brief
 *
 * @param rb
 */
void bkup_load_sysmode(uint32_t *mode)
{
    LOG_INFO("bkup_load_sysmode");
    std::memcpy(mode, &m_backup_map.map.sys_mode, sizeof(uint32_t));
}

/**
 * @brief
 *
 * @param counter
 * @param mark
 */
void bkup_event_wdt_save(uint8_t counter, time_t mark)
{
    std::memcpy(&counter, &m_backup_map.map.event_cnt, sizeof(counter));
    std::memcpy(&mark, &m_backup_map.map.event_mark, sizeof(mark));
}

/**
 * @brief
 *
 * @param counter
 * @param mark
 */
void bkup_event_wdt_load(uint8_t *counter, time_t *mark)
{
    std::memcpy(&m_backup_map.map.event_cnt, counter, sizeof(counter));
    std::memcpy(&m_backup_map.map.event_mark, mark, sizeof(mark));
}