/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdio>

#include "app/utils/build_marks.h"
#include "app/version.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME main
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Global
int main(void)
{
    // iomock_init();
    LOG_INFO("Emu started...");
    LOG_INFO("Version: %s", FW_VERSION);
    LOG_INFO("Git hash: %s", bm_get_fw_hash());
    LOG_INFO("Build time: %s", bm_get_fw_build_timestamp());
    while (1)
    {
        // application();
        // while (sys_get_wakeup_cause().d32 == 0)
        // {
        //     delay_ms(1);
        // }
        // printf("\n\n");
    }
}
//<<----------------------
