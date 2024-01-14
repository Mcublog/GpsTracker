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
#include <cstring>

#include "app/application.h"
#include "app/interfaces/System.hpp"
#include "app/io/gpio/gpio.h"
#include "app/utils/build_marks.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "app/version.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME wkptst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    LOG_RAW_INFO("\n");
    LOG_INFO("Version: %s", FW_VERSION);
    LOG_INFO("Git hash: %s", bm_get_fw_hash());
    LOG_INFO("Build time: %s", bm_get_fw_build_timestamp());

    constexpr uint32_t kMaxBuffSize = 128;
    const uint32_t kRegsSize = tu_get_backup_reg_size_max();
    uint8_t wbuff[kMaxBuffSize] ={0};

    LOG_INFO("Reg number: %d size in bytes: %d", kRegsSize / sizeof(uint32_t), kRegsSize);

    for (uint32_t i = 0; i < kRegsSize; i++)
        wbuff[i] = i + 1;

    tu_write_to_backup_reg(wbuff, kRegsSize);

    uint8_t rbuff[kMaxBuffSize] = {0};
    tu_read_from_backup_reg(rbuff, kRegsSize);

    if (std::memcmp(wbuff, rbuff, kRegsSize) != 0)
    {
        LOG_ERROR("std::memcmp(wbuff, rbuff, kRegsSize) != 0");
        System::infitite_loop();
    }

    bool red = false;
    while (1)
    {
        delay_ms(125);
        io_gpio_red_led(red ^=true);
    }
}