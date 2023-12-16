
#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#include "app/io/serial/Serial.hpp"

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
    System() {};

public:
    System(const System&) = delete;
    System& operator=(const System &) = delete;
    System(System &&) = delete;
    System & operator=(System &&) = delete;

    static auto& instance(){
        static System m_system;
        return m_system;
    }

    virtual bool go_to_stanby(void);
    wakeup_cause_t get_wakeup_cause(void);
    bool is_wake_up_event(void);

    void mode_set(sys_mode_t mode);
    sys_mode_t mode_get(void);

    Serial *get_serial_device(void);

    virtual void set_performance(sys_performance_t perf);
    virtual sys_performance_t get_performance(void);
    virtual void reinit_peripheral(void);

    void infitite_loop(void);
};






#endif // SYSTEM_H