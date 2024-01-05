
#ifndef STORAGE_BACKUP_H
#define STORAGE_BACKUP_H

#include <array>
#include <cstdbool>
#include <ctime>

#include "app/system/config.h"

#ifdef __cplusplus
extern "C"
{
#endif

    bool bkup_is_active(void);

    void bkup_set_active(void);
    void bkup_reset_active(void);

    void bkup_save_sysmode(uint32_t *mode);
    void bkup_load_sysmode(uint32_t *mode);

    void bkup_event_wdt_save(uint8_t counter, time_t mark);
    void bkup_event_wdt_load(uint8_t *counter, time_t *mark);

#ifdef __cplusplus
}
#endif

#endif // STORAGE_BACKUP_H