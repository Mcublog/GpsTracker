#ifndef APP_CONFIG_TYPES_H
#define APP_CONFIG_TYPES_H

#include <stdint.h>
#include <time.h>

// wwdt - Working WatchDog Timer
typedef struct
{
    uint32_t version;           // version of config
    uint32_t event_diff_time_s; // log event time diff
    uint32_t to_stanby_time_s;  // time when no events are received
    uint32_t threshold;         // number of events in period for the change work mode
    uint32_t manual_mode;       // working by switch
    uint8_t reserved[44];
} log_config_t; // Data logging configuration

typedef struct
{
    uint32_t version;         // version of config
    uint32_t experied_time_s; // time to expired wwdt
    uint32_t to_stanby_time_s;
    uint32_t threshold;
    uint8_t reserved[52];
} wwdt_config_t;

typedef struct
{
    uint32_t version; // version of config
    uint32_t serial;  // serial number of device
    uint8_t reserved[8];
} serial_num_t;

typedef struct
{
    uint32_t last;      // inner mark for config detection
    uint32_t version;   // version of config
    log_config_t log;   // log configuration
    wwdt_config_t wwdt; // wwdt config
    serial_num_t s;     // serial number
    uint32_t crc32;
} config_t __attribute__((aligned(4)));

#endif // APP_CONFIG_TYPES_H
