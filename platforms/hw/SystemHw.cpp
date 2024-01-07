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
#include "app/gps/GnssParser.hpp"
#include "app/proto/cobs/Parser.hpp"
#include "app/utils/delay.h"
#include "platforms/hw/SystemHw.hpp"
#include "platforms/hw/stm32/UartSerial.hpp"
#include "platforms/hw/stm32/UsbSerial.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME hwsys
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//>>---------------------- Locals
static USBSerial m_sdev = USBSerial();
static UartSerial m_gps_sdev = UartSerial();
static Parser m_cobsp = Parser();
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
 * @return Parser*
 */
Parser *SystemHW::cobs_parser()
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