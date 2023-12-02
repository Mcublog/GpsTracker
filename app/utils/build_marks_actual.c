/**
 * @file build_marks.c.in
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Specific function for getting hash.
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/utils/build_marks.h"

/**
 * @brief Returns the hash of git commit
 *
 * @return const char*
 */
const char *bm_get_fw_hash(void)
{
    return "864d44f";
}

/**
 * @brief Returns the build timestamp
 *
 * @return const char*
 */
const char *bm_get_fw_build_timestamp(void)
{
    return "2023-12-02T17:10:33";
}
