/**
 * @file SystemEmu.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Implementaion of Emulator system
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include "platforms/hw/SystemHw.hpp"
#include "app/proto/cobs/Parser.hpp"
#include "app/proto/nmea/Parser.hpp"
#include "app/utils/delay.h"
#include "platforms/hw/stm32/UartSerial.hpp"
#include "platforms/hw/stm32/UsbSerial.hpp"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rtc.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME hwsys
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//>>---------------------- Locals
static USBSerial m_sdev = USBSerial();
static UartSerial m_gps_sdev = UartSerial();
static CobsParser m_cobsp = CobsParser();
static GnssParser m_gnssp = GnssParser();
//<<----------------------
void SystemHW::init()
{
    m_cobsp.init(&m_sdev);
    m_gnssp.init(&m_gps_sdev);
}
/**
 * @brief Print system type in log
 *
 */
void SystemHW::what(void)
{
    LOG_INFO("it is HW System");
}

Serial *SystemHW::communication_serial()
{
    return (Serial *)&m_sdev;
}
/**
 * @brief
 *
 * @return CobsParser*
 */
CobsParser *SystemHW::cobs_parser()
{
    return &m_cobsp;
}

/**
 * @brief
 *
 * @return GnssParser*
 */
GnssParser *SystemHW::gnss_parser()
{
    return &m_gnssp;
}

bool SystemHW::go_to_stanby()
{
    LOG_INFO("go to stanby mode");

    // LL_RTC_ClearFlag_WUT(hrtc.Instance);
    // LL_RTC_ClearFlag_ALRA(hrtc.Instance);
    // LL_RTC_ClearFlag_ALRB(hrtc.Instance);

    LL_PWR_ClearFlag_WU();
    LL_PWR_ClearFlag_SB();
    // TODO: разобраться почему с этой строчкой виснет в релизе
#ifdef NDEBUG
    HAL_DBGMCU_DisableDBGStandbyMode();
    // HAL_DBGMCU_EnableDBGStandbyMode();
#else
    HAL_DBGMCU_EnableDBGStandbyMode();
#endif

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_EnterSTANDBYMode();
    return false;
}