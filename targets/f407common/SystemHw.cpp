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

#include "app/utils/delay.h"
#include "targets/f407common/SystemHw.hpp"
#include "app/proto/cobs/Parser.hpp"
// #include "targets/desktop/impl/Sdev.hpp"
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

// static void *m_receiving_data(void*);

// static SDevice m_sdev = SDevice("/dev/ttyS11", m_receiving_data);
// static SDevice m_gps_sdev = SDevice();
static Parser m_cobsp = Parser();
// static AccEmu m_acc = AccEmu();
// static SensorEmu m_sensor = SensorEmu();
// static EmuVoltage m_voltage = EmuVoltage();


// static uint8_t m_input_buffer[INPUT_BUFFER_SIZE];
// static uint8_t m_output_buffer[OUTPUT_BUFFER_SIZE];

// static ios_ctl_t m_ctl = {
//     {m_input_buffer,    INPUT_BUFFER_SIZE},
//     {m_output_buffer,   OUTPUT_BUFFER_SIZE},
//     nullptr
// };

// static void *m_receiving_data(void*)
// {
    // while (1)
    // {
    //     uint8_t byte = 0;
    //     long size = read(m_sdev.m_io_stream, &byte, 1);
    //     if (size <= 0)
    //     {
    //         delay_ms(1);
    //         continue;
    //     }
    //     ios_chunk_t data = {&byte, 1};
    //     m_sdev.IrqHandler(&data);
    // }
// }
//<<----------------------
void SystemHW::init()
{

    // m_cobsp.init(&m_sdev);
    // m_gps_sdev.Init(&m_ctl);
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
    //return &m_sdev;
    return nullptr;
}
/**
 * @brief
 *
 * @return Parser*
 */
Parser *SystemHW::cobs_parser()
{
    // return &m_cobsp;
    return nullptr;
}