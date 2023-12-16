#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

typedef enum {
    TU_ALARM_0 = 0,
    TU_ALARM_LAST
}tu_alarm_number_t;

typedef struct
{
    tu_alarm_number_t number;
    void (*callback)(void);
}tu_alarm_t;

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------- Definitions --------------------------
#define TIME_ON_RESET_STATE (946684800U)
#define TIME_FOR_CHECK_SYNC (1577836800U) // 01/01/2020 @ 00:00
//--------------------------------------------------------------

void tu_init(void);
void tu_set_time(const time_t *timestamp);
time_t tu_get_current_time(void);
time_t tu_get_current_time_with_fraction(uint32_t *ms);
void tu_set_alarm(const time_t *time);
time_t tu_get_alarm_time(void);
void tu_reset_alarm(void);
void tu_write_to_backup_reg(void *data, uint32_t size);
void tu_read_from_backup_reg(void *data, uint32_t size);

bool tu_alarm_set_handlers(const tu_alarm_t *alarms, uint32_t size);
void tu_alarm_irq_handler(tu_alarm_number_t number);

const char *tu_print_current_time_full(void);
const char *tu_print_time_full(const time_t *time);
const char *tu_print_current_time_only(void);
const char *tu_print_time_only(const time_t *time);

bool tu_is_rtc_alarm_cause(void);
void tu_rtc_alarm_flag_reset(void);


#ifdef __cplusplus
}
#endif

#endif //TIME_UTILS_H
