#ifndef LIS3D_CTRL_H
#define LIS3D_CTRL_H

#include <stdint.h>

#include "lis3dh_driver.h"
// #include "report_data_types.h"

typedef struct
{
    LIS3DH_ODR_t freq;
    LIS3DH_Fullscale_t full_scale;
    uint32_t acc_size;
    uint32_t th;
    uint32_t th_alert;
}lis_cfg_t;

typedef enum
{
	LIS_TRHLD,
	LIS_DDRY1
}lis3_int_t;

typedef enum
{
    LIS3DH_NOT_ALERT = 0,
    LIS3DH_ALERT
} lis3dh_alert_status_t;

void lis3d_single_mode(void);
void lis3d_set_cfg(const lis_cfg_t *cfg);
void lis3d_irq_mode(const lis_cfg_t *cfg, lis3_int_t irq);
void lis3d_mode_old(LIS3DH_ODR_t ov, LIS3DH_Fullscale_t fs, u8_t ths, lis3_int_t irq);

uint32_t lis3d_get_freq_hz(LIS3DH_ODR_t f);
uint32_t lis3d_get_fullscale_g(LIS3DH_Fullscale_t fs);

LIS3DH_ODR_t lis3d_get_freq_lis(uint32_t f_hz);
LIS3DH_Fullscale_t lis3d_get_fullscale_lis(uint32_t f_g);

int16_t lis3d_get_th_lis(uint32_t th_mg, LIS3DH_Fullscale_t s);
int32_t lis3d_get_th_mg(int16_t th, LIS3DH_Fullscale_t s);

lis3dh_alert_status_t lis3d_check_alert(const AxesRaw_t* d, const lis_cfg_t *cfg);

float lis3d_get_axis_f(int16_t axis, LIS3DH_Fullscale_t fs);


void acc_cfg_printf(const lis_cfg_t *cfg);


#endif
