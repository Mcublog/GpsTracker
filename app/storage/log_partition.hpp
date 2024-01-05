
#ifndef APP_STORAGE_LOG_PARTITION_HPP
#define APP_STORAGE_LOG_PARTITION_HPP

#include "libs/ringfs-0.2.0/ringfs.h"

typedef void (*busy_callback_t)(void);

#ifdef __cplusplus
extern "C"
{
#endif

    void log_partition_init(void);
    const ringfs_flash_partition_t *log_acc_partition(void);
    const ringfs_flash_partition_t *log_sensor_partition(void);

    void log_partition_set_busy_callback(busy_callback_t busy_callback);

    bool log_partition_is_idle(void);

#ifdef __cplusplus
}
#endif

#endif // APP_STORAGE_LOG_PARTITION_HPP