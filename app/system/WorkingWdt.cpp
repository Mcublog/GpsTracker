/**
 * @file WorkingWdt.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-01-03
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <cstdint>

#include "app/storage/backup.hpp"
#include "app/system/WorkingWdt.hpp"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME wwdt
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief
 *
 */
void WorkingWdt::event_getting()
{
    m_last_mark = tu_get_current_time();
    m_event_counter++;
}

/**
 * @brief
 *
 */
void WorkingWdt::reset()
{
    m_event_counter = 0;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool WorkingWdt::is_expired()
{
    return tu_get_current_time() > (m_last_mark + kSleepPeriodS);
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool WorkingWdt::is_treshold()
{
    return m_event_counter > kEventThreshold;
}

/**
 * @brief
 *
 */
void WorkingWdt::save() const
{
    bkup_event_wdt_save(m_event_counter, m_last_mark);
}

/**
 * @brief
 *
 */
void WorkingWdt::load()
{
    bkup_event_wdt_load(&m_event_counter, &m_last_mark);
    if (m_last_mark == 0)
        m_last_mark = tu_get_current_time();
}