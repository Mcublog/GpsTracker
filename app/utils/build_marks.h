
#ifndef APP_UTILS_BUILD_MARKS_H
#define APP_UTILS_BUILD_MARKS_H

#ifdef __cplusplus
extern "C"
{
#endif

    const char *bm_get_fw_hash(void);
    const char *bm_get_fw_build_timestamp(void);

#ifdef __cplusplus
}
#endif

#endif // APP_UTILS_BUILD_MARKS_H