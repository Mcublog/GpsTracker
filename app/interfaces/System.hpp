
#ifndef APP_SYSTEM_INTERFACES_SYSTEM_H
#define APP_SYSTEM_INTERFACES_SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#include "app/system/common.h"
#include "app/interfaces/Serial.hpp"
#include "app/proto/cobs/Parser.hpp"

typedef union
{
    uint32_t d32;
    struct
    {
        uint8_t by_rtc              : 1;
        uint8_t by_external_power   : 1;
        uint8_t by_accel            : 1;
        uint32_t reserved3_31       : 29;
    }field;

} wakeup_cause_t;

typedef enum
{
    DELAYED_START = 0,
    FIRST_RUN,
    NORMAL,
    IDLE
} sys_mode_t;

typedef enum
{
    LOW_POWER,
    NOMINAL
} sys_performance_t;

class System
{
private:

public:
    virtual void init() = 0;

    virtual void what() = 0;

    virtual bool go_to_stanby();
    wakeup_cause_t get_wakeup_cause();
    bool is_wake_up_event();

    void mode_set(sys_mode_t mode);
    sys_mode_t mode_get();

    virtual Serial *communication_serial() = 0;
    virtual Parser *cobs_parser() = 0;

    virtual void set_performance(sys_performance_t perf);
    virtual sys_performance_t get_performance();
    virtual void reinit_peripheral();

    void infitite_loop();
};

#endif // APP_SYSTEM_INTERFACES_SYSTEM_H