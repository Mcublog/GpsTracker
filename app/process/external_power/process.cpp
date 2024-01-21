/**
 * @file process.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-17
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstring>

#include "app/io/gpio/gpio.h"
#include "app/process/external_power/process.hpp"
#include "app/proto/cobs/Parser.hpp"
#include "app/proto/commands.h"
#include "app/proto/nmea/types.h"
#include "app/system/system.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "app/storage/common.hpp"
#include "app/storage/GnssLog.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME comm
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"

 //>>---------------------- Locals
CobsParser *m_parser = nullptr;
GnssParser *m_gnssp = nullptr;
GnssLog m_log;

static bool m_command_handler(const command_t *command)
{
    LOG_INFO("handle: id: %#x channel: %#x", command->id, command->channel);
    if (command->id == CMDID_SET_RTC)
        tu_set_time((time_t *)command->data);
    // else if (command->id == CMDID_SET_SETTINGS)
    // {
    //     config_t *config = (config_t *)(command->data);
    //     dprint_config(config);
    //     config_save(config);
    // }

    uint32_t limit = 0;
    uint8_t *output = isystem()->cobs_parser()->get_output_buffer(&limit);
    std::memset(output, 0, sizeof(command_ack_t));
    isystem()->cobs_parser()->write_message((uint8_t *)output, sizeof(command_ack_t));
    return true;
}

static bool m_get_report_handler(const command_t *command)
{
    uint32_t limit = 0;
    command_t *output = reinterpret_cast<command_t *>(
        isystem()->cobs_parser()->get_output_buffer(&limit));

    LOG_INFO("handle: id: %#x channel: %#x max size: %d", command->id, command->channel,
             limit);
    std::memset(output, 0, limit);

    output->id = command->id;
    output->channel = command->channel;

    uint32_t kMaxRecords = (limit - sizeof(command_t)) / sizeof(gnss_record_v1_t);

    int readed = m_log.pop(output->data, kMaxRecords);
    int size = readed == 0 ? sizeof(command_ack_t) : readed * sizeof(gnss_record_v1_t);
    LOG_INFO("Read/ max record in packet: %d/%d", readed, kMaxRecords);
    if (readed == 0)
        m_log.rewing();

    isystem()->cobs_parser()->write_message((uint8_t *)output, size);
    return true;
}

static const command_list_item_t m_command_list[CMDID_LAST] = {
    {CMDID_SET_SETTINGS, m_command_handler},
    {CMDID_GET_SETTINGS, m_command_handler},
    {CMDID_SET_RTC, m_command_handler},
    {CMDID_GET_RTC, m_command_handler},
    {CMDID_GET_REPORTS, m_get_report_handler}};

//<<----------------------

bool ExtPower::process(void)
{
    LOG_INFO("ExtPower::process: run");

    m_parser = isystem()->cobs_parser();
    m_gnssp = isystem()->gnss_parser();

    m_log.init();
    m_log.set_long_busy_callback(NULL);
    m_log.rewing();

    command_parser_list_init((const command_list_item_t *)&m_command_list);

    while (io_read_external_power_pin())
    {
        if (m_gnssp->is_message_received())
        {
            lwgps_t *gnss = m_gnssp->read_message();
            LOG_DEBUG("Valid status: %d: %s", gnss->is_valid, tu_print_current_time_only());
            LOG_DEBUG("Time: %02d:%02d:%02d", gnss->hours, gnss->minutes, gnss->seconds);
            LOG_DEBUG("Latitude: %f degrees", gnss->latitude);
            LOG_DEBUG("Longitude: %f degrees", gnss->longitude);
            LOG_DEBUG("Altitude: %f meters", gnss->altitude);
        }

        if (m_parser->is_message_received() == false)
        {
            delay_ms(1);
            continue;
        }

        ios_message_t *msg = m_parser->read_message();
        command_parser((const command_t *)msg->data);
    }
    sys_mode_t mode =
        config().log.manual_mode ? sys_mode_t::AUTONOMOUS : sys_mode_t::IDLE;
    isystem()->mode_set(mode);
    return false;
}