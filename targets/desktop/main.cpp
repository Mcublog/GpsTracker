/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdio>
#include <cstring>

#include "app/application.h"
#include "app/system/system.h"
#include "app/utils/build_marks.h"
#include "app/utils/delay.h"
#include "app/version.h"
#include "targets/desktop/io_mock/io_mock.h"
// libs
#include "lwgps/lwgps.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME main
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Global

/* GPS handle */
lwgps_t hgps;

/**
 * \brief           Dummy data from GPS receiver
 */
const char gps_rx_data[] = ""
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

int main(void)
{
    // iomock_init();
    LOG_INFO("Emu started...");
    LOG_INFO("Version: %s", FW_VERSION);
    LOG_INFO("Git hash: %s", bm_get_fw_hash());
    LOG_INFO("Build time: %s", bm_get_fw_build_timestamp());

    /* Init GPS */
    lwgps_init(&hgps);
    /* Process all input data */
    lwgps_process(&hgps, gps_rx_data, strlen(gps_rx_data));

    /* Print messages */
    LOG_INFO("Valid status: %d", hgps.is_valid);
    LOG_INFO("Latitude: %f degrees", hgps.latitude);
    LOG_INFO("Longitude: %f degrees", hgps.longitude);
    LOG_INFO("Altitude: %f meters", hgps.altitude);

    iomock_init();
    while (1)
    {
        application();
        while (instance().get_wakeup_cause().d32 == 0)
        {
            delay_ms(1);
        }
        printf("\n\n");
    }
}
//<<----------------------
