#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <stdint.h>
#include <time.h>


typedef struct
{
    int32_t min;
    int32_t max;
} int32_th;

typedef struct
{
    uint32_t period_s;
    int32_th temperature_threshold; // int xx.xx * 1000 C temp
    int32_th humidity_threshold;   // (%)
    int32_th pressure_threshold;   // (mBar)
} tph_cfg_t;                       // temperature, pressure, humidity config

typedef struct
{
    uint8_t     freq; // descrete frequncy
    uint8_t     full_scale;
    uint32_t    acc_size;
    uint32_t    pre_threshold;   // threshold of start
    uint32_t    alert_threshold; // alert threshold
} accel_cfg_t;                // accelerometer config

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} accel_ofst_t;

typedef struct
{
    accel_cfg_t     accel;
    tph_cfg_t       tph_cfg;
    time_t          time_to_start; // time to start logging
    time_t          time_to_stop;  // time to stop logging
} rpt_config_t;

typedef struct
{
    uint32_t serial;
} serial_num_t;

typedef struct
{
    uint32_t        last;
    rpt_config_t    rpt_cfg;
    serial_num_t    s;
    accel_ofst_t    aoffst;
    uint32_t        crc32;
} config_t __attribute__((aligned(4)));

#endif // CONFIG_TYPES_H
