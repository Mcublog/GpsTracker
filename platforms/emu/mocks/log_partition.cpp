/**
 * @file log_partition.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Mocking for log partition
 * @version 0.1
 * @date 2023-09-26
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/storage/log_partition.hpp"
#include "libs/ringfs-0.2.0/tests/flashsim.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME mlogp
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Local vars
static struct flashsim *sim;

static constexpr int kSectorSize = 4096;
static constexpr int kNumberSectorInOneChip = 16;
static constexpr int kMaxMemoryChips = 1;

static busy_callback_t m_busy_callback = NULL;

bool ll_init = false;
//<<----------------------

//>>---------------------- Local function
static int op_sector_erase(uint32_t address)
{
    flashsim_sector_erase(sim, address);
    return 0;
}

static ssize_t op_program(uint32_t address, const void *data, size_t size)
{
    flashsim_program(sim, address, (uint8_t*)data, (int)size);
    return size;
}

static ssize_t op_read(uint32_t address, void *data, size_t size)
{
    flashsim_read(sim, address, (uint8_t*)data, (int)size);
    return size;
}

// clang-format off
const static ringfs_flash_partition_t m_gnss_partition =
{
    /**< Sector size, in bytes. */
    .sector_size = kSectorSize,
    /**< Partition offset, in sectors. */
    .sector_offset = (kNumberSectorInOneChip * kMaxMemoryChips / 2),
    /**< Partition size, in sectors. */
    .sector_count = (kNumberSectorInOneChip * kMaxMemoryChips / 2),

    .sector_erase = op_sector_erase,
    .program = op_program,
    .read = op_read,
};
// clang-format on

static void fixture_flashsim_setup(void)
{
    sim = flashsim_open("ringfs.sim",
                        kSectorSize * kNumberSectorInOneChip * kMaxMemoryChips,
                        kSectorSize);
}

static void fixture_flashsim_teardown(void)
{
    flashsim_close(sim);
    sim = NULL;
}
//<<----------------------

void log_partition_init(void)
{
    if (ll_init)
        return;
    fixture_flashsim_setup();
    ll_init = true;
}

/**
 * @brief
 *
 * @return ringfs_flash_partition_t
 */
const ringfs_flash_partition_t *log_gnss_partition(void)
{
    return &m_gnss_partition;
}

/**
 * @brief
 *
 * @param busy_callback
 */
void log_partition_set_busy_callback(busy_callback_t busy_callback)
{
    m_busy_callback = busy_callback;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool log_partition_is_idle(void)
{
    return true;
}