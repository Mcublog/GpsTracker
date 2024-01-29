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
#include <chrono>
#include <unistd.h>

#include "app/application.h"
#include "app/io/serial/cobs/serial.h"
#include "app/io/serial/types.h"
#include "app/proto/cobs/Parser.hpp"
#include "app/proto/commands.h"
#include "app/proto/nmea/types.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "platforms/emu/implementation/Sdev.hpp"
#include "platforms/emu/io/files.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME dcobs
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals vars
static void *read_process(void*);
static SDevice sdev_rx = SDevice("/dev/ttyS11", read_process);
static CobsParser parser_rx;
//<<----------------------

//>>---------------------- Private
static void *read_process(void*)
{
    LOG_RAW_INFO("\r\n");
    while (1)
    {
        uint8_t byte = 0;
        long size = read(sdev_rx.m_io_stream, &byte, 1);
        if (size <= 0)
        {
            delay_ms(1);
            continue;
        }
        LOG_RAW_INFO("\\x02%x", byte);
        ios_chunk_t data = {&byte, 1};
        sdev_rx.irq_handler(&data);
    }
    return nullptr;
}


static uint32_t serial_irq(ios_chunk_t *data)
{
    return parser_rx.irq_handler(data);
}
//<<----------------------

/**
 * @brief Test application
 *
 */
int main(void)
{
    iomock_init();
    SDevice sdev_tx = SDevice("/dev/ttyS10", nullptr);

    CobsParser parser_tx;
    parser_tx.init(&sdev_tx);

    parser_rx.init(&sdev_rx);
    sdev_rx.register_irq(serial_irq);

    uint32_t limit = 0;
    command_ack_t *ack =
        reinterpret_cast<command_ack_t *>(parser_tx.get_output_buffer(&limit));
    ack->channel = 1;
    ack->status = 2;

    parser_tx.write_message(11, 22, reinterpret_cast<uint8_t *>(ack), sizeof(command_ack_t));

    auto start = std::chrono::system_clock::now();

    while (parser_rx.is_message_received() == false)
    {
        if (std::chrono::system_clock::now()-start > std::chrono::duration<int>(5))
        {
            LOG_ERROR("parser_rx.is_message_received() == false");
            return -1;
        }
    }

    ios_message_t *msg = parser_rx.read_message();
    LOG_INFO("rx: ch: %d version: %d size: %d ", msg->head.channel, msg->head.version,
         msg->head.size);

    return 0;
}