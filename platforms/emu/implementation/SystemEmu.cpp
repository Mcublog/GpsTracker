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
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "app/proto/cobs/Parser.hpp"
#include "app/proto/nmea/Parser.hpp"
#include "app/utils/delay.h"
#include "platforms/emu/implementation/Sdev.hpp"
#include "platforms/emu/implementation/SystemEmu.hpp"
// #include "targets/desktop/mocks/AccEmu.hpp"
// #include "targets/desktop/mocks/SensorEmu.hpp"
// #include "targets/desktop/mocks/EmuVoltage.hpp"

//>>---------------------- Log control
#define LOG_MODULE_NAME esys
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
#define INPUT_BUFFER_SIZE   (256)
#define OUTPUT_BUFFER_SIZE  (256)

static void *communication_reading(void*);
static void *gnss_reading(void*);

static SDevice m_sdev = SDevice("/dev/ttyS11", communication_reading);
static SDevice m_gps_sdev = SDevice("/dev/ttyS13", gnss_reading);
static CobsParser m_cobsp = CobsParser();
static GnssParser m_gnssp = GnssParser();
// static AccEmu m_acc = AccEmu();
// static SensorEmu m_sensor = SensorEmu();
// static EmuVoltage m_voltage = EmuVoltage();

static void read_process(SDevice *ser)
{
    while (1)
    {
        uint8_t byte = 0;
        long size = read(ser->m_io_stream, &byte, 1);
        if (size <= 0)
        {
            delay_ms(1);
            continue;
        }
        ios_chunk_t data = {&byte, 1};
        ser->IrqHandler(&data);
    }
}

static void *communication_reading(void*)
{
    read_process(&m_sdev);
    return nullptr;
}

static void *gnss_reading(void*)
{
    read_process(&m_gps_sdev);
    return nullptr;
}
//<<----------------------
void SystemEmu::init()
{

    m_cobsp.init(&m_sdev);
    m_gnssp.init(&m_gps_sdev);
}
/**
 * @brief Print system type in log
 *
 */
void SystemEmu::what(void)
{
    LOG_INFO("it is Emulator System");
}

Serial *SystemEmu::communication_serial()
{
    return &m_sdev;
}
/**
 * @brief
 *
 * @return CobsParser*
 */
CobsParser *SystemEmu::cobs_parser()
{
    return &m_cobsp;
}

/**
 * @brief
 *
 * @return GnssParser*
 */
GnssParser *SystemEmu::gnss_parser()
{
    return &m_gnssp;
}
