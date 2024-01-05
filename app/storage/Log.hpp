
#ifndef APP_STORAGE_LOG_H
#define APP_STORAGE_LOG_H

#include <stdint.h>

#include "app/storage/log_partition.hpp"
#include "libs/ringfs-0.2.0/ringfs.h"

typedef void (*backup_load_t)(ringfs_t *fs);

class Log
{
  protected:
    ringfs_t m_fs = {};
    static void ring_fs_init(const ringfs_flash_partition_t *partition,
                             const uint32_t chunk_size, ringfs_t *fs,
                             backup_load_t load_from_backup);

    static bool is_idle(void);
    static void idle_waiting(void);

  public:
    virtual void init();
    virtual void save_to_backup() = 0;

    void set_long_busy_callback(busy_callback_t busy_callback);
    void append(const void *record);
    int pop(void *record);
    void discard(void);
    void format(void);
    void rewing(void);
    void usage(uint32_t *current, uint32_t *capacity);
    bool is_last_record(void);
    bool is_empty();

};

#endif // APP_STORAGE_LOG_H