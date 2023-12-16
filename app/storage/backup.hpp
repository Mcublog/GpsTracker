
#ifndef STORAGE_BACKUP_H
#define STORAGE_BACKUP_H

#include <array>
#include <stdbool.h>

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

#ifdef __cplusplus
}
#endif

#endif // STORAGE_BACKUP_H