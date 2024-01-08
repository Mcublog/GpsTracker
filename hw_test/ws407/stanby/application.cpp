/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-23
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdint>

#include "app/application.h"
#include "app/gps/data_types.h"
#include "app/io/gpio/gpio.h"
#include "app/storage/GnssLog.hpp"
#include "app/interfaces/System.hpp"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME logtst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    LOG_INFO("LOG test");

    GnssLog log = GnssLog();
    log.init();
    log.set_long_busy_callback(NULL);

    static constexpr uint8_t kMaxDataSize = 10;
    for (auto i = 0; i < kMaxDataSize; i++)
    {
        gnss_record_t dummy_data;
        dummy_data.tm = i;
        log.append(&dummy_data);
    }
    uint32_t current = 0, capacity = 0;

    log.usage(&current, &capacity);
    LOG_INFO("state: %d/%d", current, capacity);
    if (current != kMaxDataSize)
    {
        LOG_ERROR("current != kMaxDataSize");
        System::infitite_loop();
    }

    for (auto i = 0; i < (kMaxDataSize >> 1); i++)
    {
        gnss_record_t dummy_data;
        log.pop(&dummy_data);
        uint32_t v = dummy_data.tm;
        LOG_INFO("pop: %d", v);
    }

    log.usage(&current, &capacity);
    LOG_INFO("state: %d/%d", current, capacity);
    if (current != kMaxDataSize)
    {
        LOG_ERROR("current != kMaxDataSize");
        System::infitite_loop();
    }

    LOG_INFO("----------- second part reading ----------");
    for (auto i = 0; i < (kMaxDataSize >> 1); i++)
    {
        gnss_record_t dummy_data;
        log.pop(&dummy_data);
        uint32_t v = dummy_data.tm;
        LOG_INFO("pop: %d", v);
    }

    log.rewing();
    log.usage(&current, &capacity);
    LOG_INFO("state after rewind: %d/%d", current, capacity);
    if (current != kMaxDataSize)
    {
        LOG_ERROR("current != kMaxDataSize");
        System::infitite_loop();
    }

    for (auto i = 0; i < kMaxDataSize; i++)
    {
        gnss_record_t dummy_data;
        log.pop(&dummy_data);
        uint32_t v = dummy_data.tm;
        LOG_INFO("pop: %d", v);
    }
    log.discard();
    log.usage(&current, &capacity);
    if (current != 0)
    {
        LOG_ERROR("current != 0");
        System::infitite_loop();
    }

    while (1)
    {
        delay_ms(1);
    }
}