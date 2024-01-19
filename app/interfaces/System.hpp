
#ifndef APP_SYSTEM_INTERFACES_SYSTEM_H
#define APP_SYSTEM_INTERFACES_SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#include "app/interfaces/Serial.hpp"
#include "app/proto/cobs/Parser.hpp"
#include "app/proto/nmea/Parser.hpp"
#include "app/system/common.h"

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
    IDLE,
    AUTONOMOUS
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
    wakeup_cause_t get_wakeup_cause() const;
    bool is_wake_up_event();

    void mode_set(sys_mode_t mode);
    sys_mode_t mode_get();
    const char *mode_stringify(sys_mode_t m);

    virtual Serial *communication_serial() = 0;
    virtual CobsParser *cobs_parser() = 0;
    virtual GnssParser *gnss_parser() = 0;

    virtual void performance_set(sys_performance_t perf);
    virtual sys_performance_t performance_get();
    const char *performance_stringify(sys_performance_t m);

    virtual void reinit_peripheral();

    static void infitite_loop();
};

#endif // APP_SYSTEM_INTERFACES_SYSTEM_H