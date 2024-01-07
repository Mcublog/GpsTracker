
#ifndef APP_GPS_DATA_TYPES_H
#define APP_GPS_DATA_TYPES_H

#include <time.h>
#include <stdint.h>

typedef struct
{
    time_t tm;
    int32_t latitude;  /*!< Latitude in units of degrees */
    int32_t longitude; /*!< Longitude in units of degrees */
    int32_t altitude;  /*!< Altitude in units of meters */
    int32_t speed;     /*!< Ground speed in knots */
    int32_t course;    /*!< Ground coarse */
    int32_t variation; /*!< Magnetic variation */
} gnss_record_t;


#endif // APP_GPS_DATA_TYPES_H