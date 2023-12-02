/**
 * @file build_marks.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/common_type.h"
#include "app/utils/build_marks.h"

/**
 * @brief
 *
 * @return const char*
 */
__weak const char *bm_get_fw_hash(void)
{
    return "@GIT_HASH@";
}

/**
 * @brief
 *
 * @return const*
 */
__weak const char *bm_get_fw_build_timestamp(void)
{
    return "@BUILD_TIMESTAMP@";
}

