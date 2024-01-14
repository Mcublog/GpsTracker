
#ifndef APP_UTILS_NMEA_H
#define APP_UTILS_NMEA_H

#include <ctime>
#include <cstdint>

#include "libs/lwgps-2.2.0/lwgps/src/include/lwgps/lwgps.h"

namespace nmea
{
time_t parse_time(const lwgps_t *gnss, int8_t tz);
};

#endif // APP_UTILS_NMEA_H