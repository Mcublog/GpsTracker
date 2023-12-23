/**
 * @file lis3d_ctrl.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Common LIS3DH driver
 * @version 0.1
 * @date 2023-08-26
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include <math.h>

#include "lis3d_ctrl.h"
#include "lis3dh_driver.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME lis3dhctrl
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local functions

/**
 * @brief Set IRQ by data ready
 *
 */
static void lis3d_int_drdy1_en(void)
{
    uint8_t value;

    LIS3DH_GetInt1Src(&value);
    LIS3DH_WriteReg(LIS3DH_INT1_CFG, 0);
    LIS3DH_SetIntConfiguration(LIS3DH_INT1_OR|
                                                    LIS3DH_INT1_ZHIE_DISABLE|LIS3DH_INT1_ZLIE_DISABLE|
                                                    LIS3DH_INT1_YHIE_DISABLE|LIS3DH_INT1_YLIE_DISABLE|
                                                    LIS3DH_INT1_XHIE_DISABLE|LIS3DH_INT1_XLIE_DISABLE);
    LIS3DH_WriteReg(LIS3DH_CTRL_REG3, 0);
    LIS3DH_WriteReg(LIS3DH_CTRL_REG3, (1<<LIS3DH_I1_DRDY1));
}

/**
 * @brief Enable
 *
 */
// static void lis3d_enable(void)
// {
//     LIS3DH_HPFAOI1Enable(MEMS_ENABLE);
//     LIS3DH_SetHPFMode(LIS3DH_HPM_NORMAL_MODE_RES);
//     LIS3DH_SetFilterDataSel((State_t)MEMS_SET);
// }

/**
 * @brief Set IRQ with threshold
 *
 */
static void lis3d_int_ths_en(void)
{
    uint8_t value;
    LIS3DH_GetInt1Src(&value);
    LIS3DH_WriteReg(LIS3DH_INT1_CFG, 0);
    LIS3DH_SetIntConfiguration(LIS3DH_INT1_OR|
                                                    LIS3DH_INT1_ZHIE_ENABLE|LIS3DH_INT1_ZLIE_ENABLE|
                                                    LIS3DH_INT1_YHIE_ENABLE|LIS3DH_INT1_YLIE_ENABLE|
                                                    LIS3DH_INT1_XHIE_ENABLE|LIS3DH_INT1_XLIE_ENABLE);
    LIS3DH_WriteReg(LIS3DH_CTRL_REG3, 0);
    LIS3DH_WriteReg(LIS3DH_CTRL_REG3, LIS3DH_I1_INT1_ON_PIN_INT1_ENABLE);
    LIS3DH_SetIntMode(LIS3DH_INT_MODE_6D_POSITION);
}

/**
 * @brief Set single mode
 *
 */
void lis3d_single_mode(void)
{
    LIS3DH_SetODR(LIS3DH_ODR_100Hz);
    LIS3DH_SetMode(LIS3DH_NORMAL);
    LIS3DH_SetFullScale(LIS3DH_FULLSCALE_2);

    LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);

    LIS3DH_SetIntConfiguration(LIS3DH_INT1_OR|
                                                    LIS3DH_INT1_ZHIE_ENABLE|LIS3DH_INT1_ZLIE_ENABLE|
                                                    LIS3DH_INT1_YHIE_ENABLE|LIS3DH_INT1_YLIE_ENABLE|
                                                    LIS3DH_INT1_XHIE_ENABLE|LIS3DH_INT1_XLIE_ENABLE);

    LIS3DH_SetInt1Pin(LIS3DH_CLICK_ON_PIN_INT1_DISABLE|
                                        LIS3DH_I1_INT1_ON_PIN_INT1_ENABLE|
                                        LIS3DH_I1_INT2_ON_PIN_INT1_DISABLE|
                                        LIS3DH_I1_DRDY1_ON_INT1_DISABLE|
                                        LIS3DH_I1_DRDY2_ON_INT1_DISABLE|
                                        LIS3DH_WTM_ON_INT1_DISABLE|
                                        LIS3DH_INT1_OVERRUN_DISABLE);

    LIS3DH_SetInt1Threshold(50);
    LIS3DH_SetIntMode(LIS3DH_INT_MODE_6D_POSITION);
}

/**
 * @brief Configure and set irq type
 *
 * @param cfg
 * @param irq
 */
void lis3d_irq_mode(const lis_cfg_t *cfg, lis3_int_t irq)
{
    LIS3DH_ResetInt1Latch();
    switch(irq)
    {
        case LIS_TRHLD:
        {
            LIS3DH_SetODR(cfg->freq);
            LIS3DH_SetMode(LIS3DH_NORMAL);
            LIS3DH_SetFullScale(cfg->full_scale);
            LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);
            LIS3DH_SetInt1Threshold(cfg->th);

            lis3d_int_ths_en();
            LIS3DH_SetBDU(MEMS_ENABLE);
        }break;

        case LIS_DDRY1:
        {
            lis3d_int_drdy1_en();
        }break;
    }
}

/**
 * @brief
 *
 * @param ov
 * @param fs
 * @param ths
 * @param irq
 */
void lis3d_mode_old(LIS3DH_ODR_t ov, LIS3DH_Fullscale_t fs, u8_t ths, lis3_int_t irq)
{
    LIS3DH_SetODR(ov);
    LIS3DH_SetMode(LIS3DH_NORMAL);
    LIS3DH_SetFullScale(fs);

    LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);

    switch(irq)
    {
        case LIS_TRHLD:
        {
            LIS3DH_SetInt1Threshold(ths);
            lis3d_int_ths_en();
            LIS3DH_SetIntMode(LIS3DH_INT_MODE_6D_POSITION);
        }break;

        case LIS_DDRY1:
        {
            lis3d_int_drdy1_en();
        }break;
    }
    LIS3DH_SetBDU(MEMS_ENABLE);
}

/**
 * @brief Configure lis3dh
 *
 * @param cfg
 */
void lis3d_set_cfg(const lis_cfg_t *cfg)
{
    LIS3DH_HPFAOI1Enable(MEMS_ENABLE);
    LIS3DH_SetHPFMode(LIS3DH_HPM_NORMAL_MODE_RES);
    LIS3DH_SetFilterDataSel((State_t)MEMS_SET);

    LIS3DH_SetODR(cfg->freq);
    LIS3DH_SetMode(LIS3DH_NORMAL);
    LIS3DH_SetFullScale(cfg->full_scale);

    LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);

    lis3d_irq_mode(cfg, LIS_TRHLD);
}

/**
 * @brief Soft reset
 *
 */
void lis3d_soft_rst(void)
{
    LIS3DH_WriteReg(LIS3DH_INT1_CFG, 0);
}

/**
 * @brief Convert odr to Herz
 *
 * @param f
 * @return uint32_t
 */
uint32_t lis3d_get_freq_hz(LIS3DH_ODR_t f)
{
    switch (f)
    {
        case LIS3DH_ODR_1Hz: return 1;
        case LIS3DH_ODR_10Hz: return 10;
        case LIS3DH_ODR_25Hz: return 25;
        case LIS3DH_ODR_50Hz: return 50;
        case LIS3DH_ODR_100Hz: return 100;
        case LIS3DH_ODR_200Hz: return 200;
        case LIS3DH_ODR_400Hz: return 400;
        case LIS3DH_ODR_1620Hz_LP: return 1620;
        case LIS3DH_ODR_1344Hz_NP_5367HZ_LP: return 1344;
    }
    return LIS3DH_ODR_1Hz;
}

/**
 * @brief Convert to g
 *
 * @param fs
 * @return uint32_t
 */
uint32_t lis3d_get_fullscale_g(LIS3DH_Fullscale_t fs)
{
    switch (fs)
    {
    case LIS3DH_FULLSCALE_2:
            return 2;
    case LIS3DH_FULLSCALE_4:
            return 4;
    case LIS3DH_FULLSCALE_8:
            return 8;
    case LIS3DH_FULLSCALE_16:
            return 16;
    }
    return LIS3DH_FULLSCALE_2;
}

/**
 * @brief Convert Herz to ODR
 *
 * @param f_hz
 * @return LIS3DH_ODR_t
 */
LIS3DH_ODR_t lis3d_get_freq_lis(uint32_t f_hz)
{
    switch (f_hz)
    {
        case 1:     return LIS3DH_ODR_1Hz;
        case 10:    return LIS3DH_ODR_10Hz;
        case 25:    return LIS3DH_ODR_25Hz;
        case 50:    return LIS3DH_ODR_50Hz;
        case 100:   return LIS3DH_ODR_100Hz;
        case 200:   return LIS3DH_ODR_200Hz;
        case 400:   return LIS3DH_ODR_400Hz;
        case 1620:  return LIS3DH_ODR_1620Hz_LP;
        case 1344:  return LIS3DH_ODR_1344Hz_NP_5367HZ_LP;
        default: return LIS3DH_ODR_400Hz;
    }
}

/**
 * @brief Conver g to LIS3DH_Fullscale_t
 *
 * @param f_g
 * @return LIS3DH_Fullscale_t
 */
LIS3DH_Fullscale_t lis3d_get_fullscale_lis(uint32_t f_g)
{
    switch (f_g)
    {
        case 2: return LIS3DH_FULLSCALE_2;
        case 4: return LIS3DH_FULLSCALE_4;
        case 8: return LIS3DH_FULLSCALE_8;
        case 16: return LIS3DH_FULLSCALE_16;
        default: return LIS3DH_FULLSCALE_2;
    }
}

/*-----------------------------------------------------------
     *  7 bit reg INT1_THS:
     *  1 LSb = 16 mg @ FS = ±2 g
     *  1 LSb = 32 mg @ FS = ±4 g
     *  1 LSb = 62 mg @ FS = ±8 g
     *  1 LSb = 186 mg @ FS = ±16 g
-----------------------------------------------------------*/
int16_t lis3d_get_th_lis(uint32_t th_mg, LIS3DH_Fullscale_t s)
{
    uint16_t th = 0;
    switch(s)
    {
        case LIS3DH_FULLSCALE_2:
        {
            th = th_mg/16;
        }break;
        case LIS3DH_FULLSCALE_4:
        {
            th = th_mg/32;
        }break;
        case LIS3DH_FULLSCALE_8:
        {
            th = th_mg/62;
        }break;
        case LIS3DH_FULLSCALE_16:
        {
            th = th_mg/186;
        }break;
    }
    return th;
}

/*-----------------------------------------------------------
     *  7 bit reg INT1_THS:
     *  1 LSb = 16 mg @ FS = ±2 g
     *  1 LSb = 32 mg @ FS = ±4 g
     *  1 LSb = 62 mg @ FS = ±8 g
     *  1 LSb = 186 mg @ FS = ±16 g
-----------------------------------------------------------*/
int32_t lis3d_get_th_mg(int16_t th, LIS3DH_Fullscale_t s)
{
    uint32_t th_mg = 0;
    switch(s)
    {
        case LIS3DH_FULLSCALE_2:
        {
            th_mg = th*16;
        }break;
        case LIS3DH_FULLSCALE_4:
        {
            th_mg = th*32;
        }break;
        case LIS3DH_FULLSCALE_8:
        {
            th_mg = th*62;
        }break;
        case LIS3DH_FULLSCALE_16:
        {
            th_mg = th*186;
        }break;
    }
    return th_mg;
}

/**
 * @brief
 *
 * @param d
 * @param cfg
 * @return lis3dh_alert_status_t
 */
lis3dh_alert_status_t lis3d_check_alert(const AxesRaw_t *d, const lis_cfg_t *cfg)
{
    LIS3DH_Fullscale_t scale = cfg->full_scale;

    float tph_alert_mg = (float)lis3d_get_th_mg(cfg->th_alert, scale) / 1000;

    float x_g = lis3d_get_axis_f(d->AXIS_X, scale);
    float y_g = lis3d_get_axis_f(d->AXIS_Y, scale);
    float z_g = lis3d_get_axis_f(d->AXIS_Z, scale);

    float e =
        sqrt((d->AXIS_X * d->AXIS_X) + (d->AXIS_Y * d->AXIS_Y) + (d->AXIS_Z * d->AXIS_Z));
    e = lis3d_get_axis_f((int)e, scale);
    if (e >= tph_alert_mg)
            return LIS3DH_ALERT;

    if (x_g < 0)
            x_g *= -1;
    if (y_g < 0)
            y_g *= -1;
    if (z_g < 0)
            z_g *= -1;

    if (x_g >= tph_alert_mg)
            return LIS3DH_ALERT;
    if (y_g >= tph_alert_mg)
            return LIS3DH_ALERT;
    if (z_g >= tph_alert_mg)
            return LIS3DH_ALERT;

    return LIS3DH_NOT_ALERT;
}

/**
 * @brief
 *
 * @param axis
 * @param fs
 * @return float
 */
float lis3d_get_axis_f(int16_t axis, LIS3DH_Fullscale_t fs)
{
        if (fs == LIS3DH_FULLSCALE_2)
        {
            return (float)axis / 15987;
        }
        if (fs == LIS3DH_FULLSCALE_4)
        {
            return (float)axis / 7840;
        }
        if (fs == LIS3DH_FULLSCALE_8)
        {
            return (float)axis / 3883;
        }
        if (fs == LIS3DH_FULLSCALE_16)
        {
            return (float)axis / 1280;
        }
        return 0;
}

/**
 * @brief
 *
 * @param cfg
 */
void acc_cfg_printf(const lis_cfg_t *cfg)
{
    LOG_INFO("===========ACC CFG============");
    LOG_INFO("freq = %d Hz", lis3d_get_freq_hz(cfg->freq));
    LOG_INFO("fs = %d G", lis3d_get_fullscale_g(cfg->full_scale));
    LOG_INFO("acc_size = %d", cfg->acc_size);
    LOG_INFO("th = %d mG", lis3d_get_th_mg(cfg->th, cfg->full_scale));
    LOG_INFO("th_alert = %d mG ", lis3d_get_th_mg(cfg->th_alert, cfg->full_scale));
    LOG_INFO("===============================");
}
