/**
 * @file main.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief data_retrieving test
 * @version 0.1
 * @date 2023-08-11
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/application.h"
#include "app/proto/nmea/types.h"
#include "app/storage/GnssLog.hpp"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "platforms/emu/io/gpio/files.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME ringfs
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
//<<----------------------

/**
 * @brief Test application
 *
 */
int main(void)
{
    iomock_init();
    tu_init(0);

    GnssLog log = GnssLog();
    log.init();
    log.set_long_busy_callback(NULL);

    static constexpr uint8_t kMaxDataSize = 10;
    for (auto i = 0; i < kMaxDataSize; i++)
    {
        gnss_record_v1_t dummy_data;
        dummy_data.tm = i;
        log.append(&dummy_data);
    }
    uint32_t current = 0, capacity = 0;

    log.usage(&current, &capacity);
    LOG_INFO("state: %d/%d", current, capacity);
    if (current != kMaxDataSize)
    {
        LOG_ERROR("current != kMaxDataSize");
        return -1;
    }

    for (auto i = 0; i < (kMaxDataSize >> 1); i++)
    {
        gnss_record_v1_t dummy_data;
        log.pop(&dummy_data);
        LOG_INFO("pop: %d", dummy_data.tm);
    }

    log.usage(&current, &capacity);
    LOG_INFO("state: %d/%d", current, capacity);
    if (current != kMaxDataSize)
    {
        LOG_ERROR("current != kMaxDataSize");
        return -1;
    }

    LOG_INFO("----------- second part reading ----------");
    for (auto i = 0; i < (kMaxDataSize >> 1); i++)
    {
        gnss_record_v1_t dummy_data;
        log.pop(&dummy_data);
        LOG_INFO("pop: %d", dummy_data.tm);
    }

    log.rewing();
    log.usage(&current, &capacity);
    LOG_INFO("state after rewind: %d/%d", current, capacity);
    if (current != kMaxDataSize)
    {
        LOG_ERROR("current != kMaxDataSize");
        return -1;
    }

    for (auto i = 0; i < kMaxDataSize; i++)
    {
        gnss_record_v1_t dummy_data;
        log.pop(&dummy_data);
        LOG_INFO("pop: %d", dummy_data.tm);
    }
    log.discard();
    log.usage(&current, &capacity);
    if (current != 0)
    {
        LOG_ERROR("current != 0");
        return -1;
    }

    return 0;
}