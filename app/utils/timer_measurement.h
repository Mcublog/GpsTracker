
#ifndef TIMER_MEASUREMENT_H
#define TIMER_MEASUREMENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void tm_accurate_checkout(void);
    void tm_reset(void);
    uint32_t tm_get_expiried_time_us(void);

    uint32_t tm_recalc_ticks_to_us(uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif // TIMER_MEASUREMENT_H