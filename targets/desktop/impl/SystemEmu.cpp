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
#include "targets/desktop/impl/SystemEmu.hpp"
#include "targets/desktop/impl/Sdev.hpp"
// #include "targets/desktop/mocks/AccEmu.hpp"
// #include "targets/desktop/mocks/SensorEmu.hpp"
// #include "targets/desktop/mocks/EmuVoltage.hpp"

//>>---------------------- Log control
#define LOG_MODULE_NAME esys
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
static SDevice m_sdev = SDevice();
// static AccEmu m_acc = AccEmu();
// static SensorEmu m_sensor = SensorEmu();
// static EmuVoltage m_voltage = EmuVoltage();
//<<----------------------

/**
 * @brief Print system type in log
 *
 */
void SystemEmu::what(void)
{
    LOG_INFO("it is Emulator System");
}

/**
 * @brief
 *
 * @return Serial*
 */
Serial *SystemEmu::get_serial_device(void)
{
    return (Serial *)&m_sdev;
}