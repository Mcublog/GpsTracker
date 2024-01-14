#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdint.h>

#include "app/config/types.h"

typedef enum {
    CONFIG_OK = 0,
    CONFIG_ERROR
} config_error_t;

#define CONFIG_CURRENT_CONFIG   (0xFFFFFFFF)
#define CONFIG_OLD_CONFIG       (0x00000000)

#ifdef __cplusplus
extern "C"
{
#endif

    config_t config_get_default(void);
    config_error_t config_save(config_t *config);
    config_error_t config_load(config_t *config);
    config_error_t config_erase(void);

#ifdef __cplusplus
}
#endif

#endif // CONFIG_SETTING_H
