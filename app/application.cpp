/**
 * @file application.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief Application entry point
 * @version 0.1
 * @date 2022-10-27
 *
 * @copyright Viacheslav mcublog (c) 2022
 *
 */
#include <cstring>
#include <stdbool.h>
#include <string>

#include "app/application.h"
#include "app/io/gpio/gpio.h"
// #include "app/mode/cobs_proto.hpp" // NOTE: cobs support proto
#include "app/system/system.h"
#include "app/utils/build_marks.h"
#include "app/utils/data_print.h"
#include "app/utils/delay.h"
#include "app/utils/time_utils.h"
#include "app/version.h"
// libs
#include "lwgps/lwgps.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME app
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Private
/* GPS handle */
static lwgps_t m_hgps;

/**
 * \brief           Dummy data from GPS receiver
 */
const char kGpsRxData[] =
    ""
    "$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
    "$GPRMB,A,,,,,,,,,,,,V*71\r\n"
    "$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
    "$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
    "$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
    "$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
    "$PGRME,22.0,M,52.9,M,51.0,M*14\r\n"
    "$GPGLL,3907.360,N,12102.481,W,183730,A*33\r\n"
    "$PGRMZ,2062,f,3*2D\r\n"
    "$PGRMM,WGS84*06\r\n"
    "$GPBOD,,T,,M,,*47\r\n"
    "$GPRTE,1,1,c,0*07\r\n"
    "$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67\r\n"
    "$GPRMB,A,,,,,,,,,,,,V*71\r\n";
//<<----------------------

/**
 * @brief
 *
 */
void application(void)
{
    LOG_RAW_INFO("\n");
    LOG_INFO("Version: %s", FW_VERSION);
    LOG_INFO("Git hash: %s", bm_get_fw_hash());
    LOG_INFO("Build time: %s", bm_get_fw_build_timestamp());

    /* Init GPS */
    lwgps_init(&m_hgps);
    /* Process all input data */
    lwgps_process(&m_hgps, kGpsRxData, strlen(kGpsRxData));

    /* Print messages */
    LOG_INFO("Valid status: %d", m_hgps.is_valid);
    LOG_INFO("Latitude: %f degrees", m_hgps.latitude);
    LOG_INFO("Longitude: %f degrees", m_hgps.longitude);
    LOG_INFO("Altitude: %f meters", m_hgps.altitude);

    System *sys = isystem();
    sys->what();

    wakeup_cause_t cause = sys->get_wakeup_cause();
    dprint_wakeup_cause(&cause);

    LOG_INFO("current time: %s", tu_print_current_time_full());
    if (cause.d32 == 0)
    {
        LOG_ERROR("EMPTY CAUSE, do nothing");
        if (sys->go_to_stanby())
            return;
        // sys_infitite_loop();
    }

    while (1)
    {
        if (cause.d32 == 0)
        {
            LOG_INFO("all work done -> go to sleep");
            if (sys->go_to_stanby())
                return;
        }

        if (cause.field.by_external_power)
        {
            // Prepare::to_external_power();
            // Scpp::process();
        }
        cause = sys->get_wakeup_cause();
    }
}