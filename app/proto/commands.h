
#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    CMDID_SET_SETTINGS = 0x00,
    CMDID_GET_SETTINGS = 0x01,
    CMDID_SET_RTC = 0x02,
    CMDID_GET_RTC = 0x03,
    CMDID_GET_REPORTS = 0x04,
    CMDID_STORAGE_CLEAR = 0x05,
    CMDID_LAST
} command_id_t;

typedef struct
{
    uint32_t channel;
    uint32_t id;
    uint8_t  data[];
} command_t;

typedef struct
{
    uint32_t channel;
    uint32_t status;
}command_ack_t;

typedef struct command_list_item
{
    uint32_t id;
    bool (*handler)(const command_t *cmd);
} command_list_item_t;

#ifdef __cplusplus
extern "C" {
#endif

    void command_parser_list_init(const command_list_item_t *command_list);
    void command_parser(const command_t *command);

#ifdef __cplusplus
}
#endif

#endif // COMMANDS_H