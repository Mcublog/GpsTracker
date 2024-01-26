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
#include "app/io/serial/types.h"
#include "app/proto/commands.h"
#include "app/proto/nmea/types.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME dcobs
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
    uint8_t buffer[64] = {0};
    ios_message_t *msg = reinterpret_cast<ios_message_t *>(buffer);
    command_t *cmd = reinterpret_cast<command_t *>(msg->data);
    msg->size = 4;
    cmd->channel = 0x01;
    cmd->id = 0x04;

    return 0;
}