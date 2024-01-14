/**
 * @file time_utils_mock.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Some utils working with a time
 * @version 0.1
 * @date 2023-02-18
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "platforms/emu/config/filelist.h"
#include "platforms/emu/io/gpio/files.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME etu
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Type definitions
#define TU_SCHEDULE_ALARM (0)
//<<----------------------

//>>---------------------- Local variables and function ----------
#define MAX_REG_ADR     (64)
static const uint32_t MAX_REG_DATA_SIZE = sizeof(uint32_t);
static const uint32_t kDefaultTickMs = 1000;

static uint8_t backup_mem[MAX_REG_ADR] = { 0 };

static pthread_t m_rtc_thread_id = 0;
static const tu_alarm_t *m_alarms = NULL;
static bool m_alarm_enabled = false;

static bool m_rtc_alarm_flag = false;

static uint32_t m_tick_delay_ms = kDefaultTickMs;
//<<----------------------

//>>---------------------- Local function definition
/**
 * @brief Emulation RTC tick
 *
 * @param callback
 * @return void*
 */
static void *m_rtc_tick(void *callback)
{
    time_t t = 0, alarm = 0;
    while(true)
    {
        iomock_read_data(RTC_TIME_FILE_NAME, &t, sizeof(t));
        if (iomock_read_data(RTC_ALARM_0_TIME_FILE_NAME, &alarm, sizeof(alarm)) == -2)
        {
            alarm = 0;
            iomock_write_data(RTC_ALARM_0_TIME_FILE_NAME, &alarm, sizeof(alarm));
        }
        t++;
        iomock_write_data(RTC_TIME_FILE_NAME, &t, sizeof(time_t));
        if (t >= alarm && m_alarm_enabled)
        {
            m_rtc_alarm_flag = true;
            tu_alarm_irq_handler(TU_ALARM_0);
        }
        delay_ms(m_tick_delay_ms);
    }
}
//<<----------------------

//>>---------------------- Global function definition -----------
/**
 * @brief
 *
 */
void tu_init(uint32_t tick_ms)
{
    if (tick_ms)
        m_tick_delay_ms = tick_ms;

    time_t reset = TIME_ON_RESET_STATE;
    if (iomock_file_is_exist(RTC_TIME_FILE_NAME) == false)
    {
        if (iomock_file_create(RTC_TIME_FILE_NAME, &reset, sizeof(reset), true) == false)
            exit(1);
    }
    if (iomock_file_is_exist(RTC_ALARM_0_TIME_FILE_NAME) == false)
    {
        if (iomock_file_create(RTC_ALARM_0_TIME_FILE_NAME, &reset, sizeof(reset), true) == false)
            exit(1);
    }
    if (m_rtc_thread_id != 0)
        return;
    pthread_create(&m_rtc_thread_id, NULL, m_rtc_tick, NULL);
}

/**
 * @brief
 *
 * @param timestamp
 */
void tu_set_time(const time_t *timestamp)
{
    iomock_write_data(RTC_TIME_FILE_NAME, timestamp, sizeof(time_t));
    LOG_DEBUG("Set RTC time: %s", ctime(timestamp));
}

/**
 * @brief
 *
 * @return time_t
 */
time_t tu_get_current_time(void)
{
    time_t t = 0;
    iomock_read_data(RTC_TIME_FILE_NAME, &t, sizeof(t));

    LOG_DEBUG("Get RTC time: %s", ctime(&t));
    return t;
}

time_t tu_get_current_time_with_fraction(uint32_t *ms)
{
    time_t t = tu_get_current_time();
    *ms = 0;
    return t;
}

/**
 * @brief
 *
 * @return char*
 */
const char *tu_print_current_time_full(void)
{
    time_t time = tu_get_current_time();
    return tu_print_time_full(&time);
}

/**
 * @brief
 *
 * @param time
 * @return char*
 */
const char *tu_print_time_full(const time_t *time)
{
    char *timestr = ctime(time);
    int32_t len = strlen(timestr);
    timestr[len - 1] = 0;
    return timestr;
}

/**
 * @brief
 *
 * @return char* -- string hh:mm:ss
 */
const char *tu_print_current_time_only(void)
{
    time_t time = tu_get_current_time();
    return tu_print_time_only(&time);
}

/**
 * @brief
 *
 * @param time
 * @return char* -- string hh:mm:ss
 */
const char *tu_print_time_only(const time_t *time)
{
    char *substr = ctime(time);
    // example of out:
    // Fri Dec 29 22:36:08 2023
    // Thu Jan  1 03:43:29 1970

    int32_t len = 0;
    int dummy[3];

    for (int32_t i = 0; i < 4; i++)
    {
        substr = strstr((const char *)substr, " ");
        substr++;
        // parsing xx:xx:xx time
        int numb = sscanf(substr, "%d:%d:%d", &dummy[0], &dummy[1], &dummy[2]);
        if (numb == 3)
            break;
    }
    len = strlen(substr);
    substr[len - 6] = 0;
    return substr;
}

/**
 * @brief Set RTC alarm
 *
 * @param time
 */
void tu_set_alarm(const time_t *time)
{
    m_alarm_enabled = true;
    iomock_write_data(RTC_ALARM_0_TIME_FILE_NAME, time, sizeof(time_t));
    LOG_INFO("set RTC alarm on: %s", tu_print_time_full(time));
}

/**
 * @brief Get current alarm time
 *
 * @return time_t
 */
time_t tu_get_alarm_time(void)
{
    time_t t = 0;

    iomock_read_data(RTC_ALARM_0_TIME_FILE_NAME, &t, sizeof(time_t));
    LOG_DEBUG("get RTC alarm time: %s", tu_print_time_full(&t));

    return t;
}

/**
 * @brief Disable RTC alarm
 *
 */
void tu_reset_alarm(void)
{
    LOG_INFO("RTC reset alarm");
    m_alarm_enabled = false;
}

/**
 * @brief Write to RTC backup registers
 *
 * @param data -- pointer to data, NOTE: data must be aligned (4)
 * @param size -- data size from 1 to @ref MAX_REG_DATA_SIZE
 */
void tu_write_to_backup_reg(void *data, uint32_t size)
{
    if (size > MAX_REG_DATA_SIZE * MAX_REG_ADR)
        return;
    if (data == NULL)
        return;
    memcpy(backup_mem, data, size);
}

/**
 * @brief Read from RTC backup registers
 *
 * @param data -- pointer to data, NOTE: data must be aligned (4)
 * @param size -- data size from 1 to @ref MAX_REG_DATA_SIZE
 */
void tu_read_from_backup_reg(void *data, uint32_t size)
{
    if (size > MAX_REG_DATA_SIZE * MAX_REG_ADR)
        return;
    if (data == NULL)
        return;
    memcpy(data, (void *)(backup_mem), size);
}

/**
 * @brief
 *
 * @param alarm
 * @param callback
 */
bool tu_alarm_set_handlers(const tu_alarm_t *alarms, uint32_t size)
{
    if (size != TU_ALARM_LAST)
    {
        LOG_ERROR("Number of alarm size is mismath TU_ALARM_LAST");
        return false;
    }
    // memcpy(m_alarms, alarms, (size * sizeof(tu_alarm_t)));
    m_alarms = alarms;
    return true;
}

/**
 * @brief Time utills irq handler
 *
 */
void tu_alarm_irq_handler(tu_alarm_number_t number)
{
    if (m_alarms == NULL)
    {
        LOG_ERROR("alarm irq handlers is not init");
        return;
    }
    for (int i = 0; i < TU_ALARM_LAST; i++)
    {
        if (m_alarms[i].number != number ||
             m_alarms[i].callback == NULL)
            continue;
        m_alarms[i].callback();
    }
}

/**
 * @brief Return rtc alarm flag
 *
 * @return true
 * @return false
 */
bool tu_is_rtc_alarm_cause(void)
{
    return m_rtc_alarm_flag;
}

/**
 * @brief Reset RTC alarm flag
 *
 */
void tu_rtc_alarm_flag_reset(void)
{
    m_rtc_alarm_flag = false;
}
//<<----------------------
