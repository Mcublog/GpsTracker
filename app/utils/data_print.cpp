/**
 * @file data_print.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-02-24
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <math.h>
#include <stdlib.h>

#include "app/utils/data_print.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME dprint
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Print wakeup cause
 *
 * @param cause
 */
void dprint_wakeup_cause(const wakeup_cause_t *cause)
{
    if (cause->d32 == 0)
    {
        LOG_INFO("EMPTY WAKE CASE");
        return;
    }
    if (cause->field.by_rtc)
        LOG_INFO("WAKE_BY_RTC");
    if (cause->field.by_external_power)
        LOG_INFO("WAKE_BY_EXTERNAL_POWER");
    if (cause->field.by_accel)
        LOG_INFO("WAKE_BY_ACCEL");
}
