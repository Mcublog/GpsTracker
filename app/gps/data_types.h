
#ifndef APP_GPS_DATA_TYPES_H
#define APP_GPS_DATA_TYPES_H

#include <time.h>
#include <stdint.h>

typedef struct
{
    time_t tm;
    double latitude;  /*!< Latitude in units of degrees */
    double longitude; /*!< Longitude in units of degrees */
    double altitude;  /*!< Altitude in units of meters */
    // double speed;     /*!< Ground speed in knots */
    // int32_t course;    /*!< Ground coarse */
    // int32_t variation; /*!< Magnetic variation */
} gnss_record_t;


#endif // APP_GPS_DATA_TYPES_H