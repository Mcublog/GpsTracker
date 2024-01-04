/**
 * @file time_utils.c
 * @author Viacheslav Konovalov (viacheslav@mcublog.xyz)
 * @brief Some utils working with a time
 * @version 0.1
 * @date 2020-09-30
 *
 * @copyright Mcublog.xyz (c) 2020
 *
 */

#include <string.h>

#include "app/utils/time_utils.h"
#include "rtc.h" // NOTE: HW specific
#include "stm32f4xx_ll_rtc.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME tu
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "app/debug/log_libs.h"
//>>---------------------- Global variables
extern RTC_HandleTypeDef hrtc; // NOTE: HW specific
//<<----------------------

//>>---------------------- Type definitions
#define TU_SCHEDULE_ALARM (RTC_ALARM_A)
//<<----------------------

//>>---------------------- Local variables and function ----------
static bool m_alarm_flag = false;
static const tu_alarm_t *m_alarms = NULL;

static const uint32_t MAX_REG_ADR = RTC_BKP_NUMBER;
static const uint32_t MAX_REG_DATA_SIZE = sizeof(uint32_t);

static void _fill_hal_time(const struct tm *time, RTC_TimeTypeDef *rtc_time);
static void _fill_hal_date(const struct tm *time, RTC_DateTypeDef *rtc_date);
static void _fill_hal_time_and_date(const struct tm *time, RTC_TimeTypeDef *rtc_time, RTC_DateTypeDef *rtc_date);
static void _fill_tm_from_hal_time(const RTC_TimeTypeDef *rtc_time, struct tm *time);
static void _fill_tm_from_hal_date(const RTC_DateTypeDef *rtc_date, struct tm *time);
static void _fill_tm_from_hal_time_and_hal_date(const RTC_TimeTypeDef *rtc_time, const RTC_DateTypeDef *rtc_date, struct tm *time);
//<<----------------------

//>>---------------------- Local function definition
static void _fill_hal_time(const struct tm *time, RTC_TimeTypeDef *rtc_time)
{
    rtc_time->Hours = time->tm_hour;
    rtc_time->Minutes = time->tm_min;
    rtc_time->Seconds = time->tm_sec;
}

static void _fill_hal_date(const struct tm *time, RTC_DateTypeDef *rtc_date)
{
    rtc_date->Year = time->tm_year - (2000 - 1900);
    rtc_date->Date = time->tm_mday;
    rtc_date->Month = time->tm_mon + 1;
}

static void _fill_hal_time_and_date(const struct tm *time, RTC_TimeTypeDef *rtc_time,
                                    RTC_DateTypeDef *rtc_date)
{
    _fill_hal_time(time, rtc_time);
    _fill_hal_date(time, rtc_date);
}

static void _fill_tm_from_hal_time(const RTC_TimeTypeDef *rtc_time, struct tm *time)
{
    time->tm_sec = rtc_time->Seconds; // seconds [0,59]
    time->tm_min = rtc_time->Minutes; // min [0,59]
    time->tm_hour = rtc_time->Hours;  // hour [0,23]
    time->tm_yday = 0;                // year day (1 january = 0) [0,365]
    time->tm_isdst = 0;               // flag Summer time (>0 - off)
}

static void _fill_tm_from_hal_date(const RTC_DateTypeDef *rtc_date, struct tm *time)
{
    time->tm_mday = rtc_date->Date;                 // mday [1,31]
    time->tm_mon = rtc_date->Month - 1;             // month [0,11]
    time->tm_year = (rtc_date->Year + 2000) - 1900; // year (1900 == 0)
    time->tm_wday = rtc_date->WeekDay - 1;          // week day (sunday = 0) [0,6]
    time->tm_yday = 0;                              // year day (1 january = 0) [0,365]
    time->tm_isdst = 0;                             // flag Summer time (>0 - off)
}

static void _fill_tm_from_hal_time_and_hal_date(const RTC_TimeTypeDef *rtc_time,
                                                const RTC_DateTypeDef *rtc_date,
                                                struct tm *time)
{
    _fill_tm_from_hal_time(rtc_time, time);
    _fill_tm_from_hal_date(rtc_date, time);
}

static void _get_rtc_time_and_date(RTC_TimeTypeDef *rtc_time, RTC_DateTypeDef *rtc_date)
{
    HAL_RTC_GetTime(&hrtc, rtc_time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, rtc_date, RTC_FORMAT_BIN);
}
//<<----------------------

//>>---------------------- Global function definition -----------
/**
 * @brief
 *
 */
void tu_init(uint32_t tick_ms)
{
}

/**
 * @brief
 *
 * @param timestamp
 */
void tu_set_time(const time_t *timestamp)
{
    struct tm time = *gmtime(timestamp);
    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    LOG_INFO("Set RTC time: %s", ctime(timestamp));
    _fill_hal_time_and_date(&time, &rtc_time, &rtc_date);
    HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
}

/**
 * @brief
 *
 * @return time_t
 */
time_t tu_get_current_time(void)
{
    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    _get_rtc_time_and_date(&rtc_time, &rtc_date);
    struct tm time =
        {
            .tm_sec = rtc_time.Seconds,               // seconds [0,59]
            .tm_min = rtc_time.Minutes,               // min [0,59]
            .tm_hour = rtc_time.Hours,                // hour [0,23]
            .tm_mday = rtc_date.Date,                 // mday [1,31]
            .tm_mon = rtc_date.Month - 1,             // month [0,11]
            .tm_year = (rtc_date.Year + 2000) - 1900, // year (1900 == 0)
            .tm_wday = rtc_date.WeekDay - 1,          // week day (sunday = 0) [0,6]
            .tm_yday = 0,                             // year day (1 january = 0) [0,365]
            .tm_isdst = 0,                            // flag Summer time (>0 - off)
        };
    time_t t = mktime(&time);
    LOG_DEBUG("Get RTC time: %s", ctime(&t));
    return t;
}

time_t tu_get_current_time_with_fraction(uint32_t *ms)
{
    RTC_TimeTypeDef rtc_time = {0};
    RTC_DateTypeDef rtc_date = {0};

    _get_rtc_time_and_date(&rtc_time, &rtc_date);
    struct tm time =
        {
            .tm_sec = rtc_time.Seconds,               // seconds [0,59]
            .tm_min = rtc_time.Minutes,               // min [0,59]
            .tm_hour = rtc_time.Hours,                // hour [0,23]
            .tm_mday = rtc_date.Date,                 // mday [1,31]
            .tm_mon = rtc_date.Month - 1,             // month [0,11]
            .tm_year = (rtc_date.Year + 2000) - 1900, // year (1900 == 0)
            .tm_wday = rtc_date.WeekDay - 1,          // week day (sunday = 0) [0,6]
            .tm_yday = 0,                             // year day (1 january = 0) [0,365]
            .tm_isdst = 0,                            // flag Summer time (>0 - off)
        };
    time_t t = mktime(&time);
    *ms = (((float)(rtc_time.SecondFraction - rtc_time.SubSeconds)) /
                 ((float)(rtc_time.SecondFraction + 1))) *
                1000;
    LOG_DEBUG("Get RTC time: %s: ms: %d", ctime(&t), *ms);
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
    int32_t len = 0;

    substr = strstr((const char *)substr, ":");
    if (substr)
        substr -= strlen("00");
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
    m_alarm_flag = false;
    struct tm t = *gmtime(time);
    RTC_AlarmTypeDef alarm =
        {
            .Alarm = TU_SCHEDULE_ALARM,
            .AlarmMask = RTC_ALARMMASK_NONE,
            .AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE,
            .AlarmDateWeekDay = t.tm_mday,
            .AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE,
        };
    _fill_hal_time(&t, &alarm.AlarmTime);
    if (NVIC_GetEnableIRQ(RTC_Alarm_IRQn) == 0)
    {
        HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
    }
    HAL_RTC_SetAlarm_IT(&hrtc, &alarm, RTC_FORMAT_BIN);
    LOG_INFO("set RTC alarm on: %s", tu_print_time_full(time));
}

/**
 * @brief Get current alarm time
 *
 * @return time_t
 */
time_t tu_get_alarm_time(void)
{
    RTC_AlarmTypeDef alarm = {0};
    RTC_DateTypeDef date = {0};
    struct tm time = {0};
    time_t t = 0;

    HAL_RTC_GetAlarm(&hrtc, &alarm, TU_SCHEDULE_ALARM, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    _fill_tm_from_hal_time_and_hal_date(&alarm.AlarmTime, &date, &time);
    t = mktime(&time);
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
    tu_rtc_alarm_flag_reset();
    HAL_RTC_DeactivateAlarm(&hrtc, TU_SCHEDULE_ALARM);
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

    uint32_t temp = 0, i = 0;
    HAL_PWR_EnableBkUpAccess();
    for (i = 0; i < size; i += sizeof(uint32_t))
    {
        memcpy(&temp, (uint32_t*)data + i, sizeof(uint32_t));
        if (i < 4)
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, temp);
        else if (i < 8)
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, temp);
        else if (i < 12)
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, temp);
        else if (i < 16)
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR3, temp);
        else if (i < 20)
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR4, temp);
    }
    // HAL_PWR_DisableBkUpAccess();
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
    HAL_PWR_EnableBkUpAccess();
    memcpy(data, (void *)(RTC_BASE + 0x50), size);
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
    m_alarm_flag = true;
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
    // Chek it LL bit
    if (m_alarm_flag == false)
    {
        m_alarm_flag = LL_RTC_IsActiveFlag_WUT(hrtc.Instance) ||
                       LL_RTC_IsActiveFlag_ALRB(hrtc.Instance) ||
                       LL_RTC_IsActiveFlag_ALRA(hrtc.Instance);
    }
    return m_alarm_flag;
}

/**
 * @brief Reset RTC alarm flag
 *
 */
void tu_rtc_alarm_flag_reset(void)
{
    LOG_DEBUG("tu_rtc_alarm_flag_reset");
    LL_RTC_ClearFlag_WUT(hrtc.Instance);
    LL_RTC_ClearFlag_ALRA(hrtc.Instance);
    LL_RTC_ClearFlag_ALRB(hrtc.Instance);
    m_alarm_flag = false;
}
//<<----------------------
