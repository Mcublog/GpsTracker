/**
 * @file timer_measurement.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief timer_measurement module
 * @version 0.1
 * @date 2023-08-26
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <stdint.h>
#include <stdbool.h>

#include "dwt.h"
#include "main.h"
#include "app/utils/timer_measurement.h"
#include "app/utils/delay.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME tm
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
// SYS_MULT calculate as example below
// SYS_MULT = (SystemCoreClock / 1000000U) = 168
static const uint32_t CHECKOUT_PERIOD_MS = 10;
static const uint32_t CHECKOUT_MULT = 1000;
static const uint32_t DELTA_US = 1000;
//<<----------------------

static uint32_t get_mult(void)
{
    return (HAL_RCC_GetHCLKFreq() / 1000000);
}

/**
 * @brief
 *
 */
void tm_accurate_checkout(void)
{
    dwt_reset();
    delay_ms(CHECKOUT_PERIOD_MS);
    uint32_t ticks = dwt_read();
    ticks = ticks / get_mult();
    uint32_t dmin = CHECKOUT_PERIOD_MS * CHECKOUT_MULT - DELTA_US;
    uint32_t dmax = CHECKOUT_PERIOD_MS * CHECKOUT_MULT + DELTA_US;
    if ((ticks >= dmin && ticks <= dmax) == false)
    {
        LOG_ERROR("accurate FAILED: %ld us != %ld", ticks,
                  CHECKOUT_PERIOD_MS * CHECKOUT_MULT);
        while (1)
        {
        };
    }
    int32_t error = (int32_t)ticks - (int32_t)(CHECKOUT_PERIOD_MS * CHECKOUT_MULT);
    if (error < 0)
        error *=(-1);
    LOG_INFO("accurate OK: %ld us == %ld error: %d.%d%%", ticks,
             CHECKOUT_PERIOD_MS * CHECKOUT_MULT,
             error / 100, error % 100);
}

/**
 * @brief
 *
 * @param ticks
 * @return uint32_t
 */
uint32_t tm_recalc_ticks_to_us(uint32_t ticks)
{
    return ticks / get_mult();
}

/**
 * @brief
 *
 */
void tm_reset(void)
{
    dwt_reset();
}

/**
 * @briefw
 *
 * @return uint32_t
 */
uint32_t tm_get_expiried_time_us(void)
{
    return tm_recalc_ticks_to_us(dwt_read());
}