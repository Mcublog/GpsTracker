
#ifndef APP_GPS_GNSSPARSER_H
#define APP_GPS_GNSSPARSER_H

#include <cstdint>
#include <cstdbool>

#include "app/interfaces/Serial.hpp"
#include "app/io/serial/types.h"

class GnssParser
{
  private:
    ios_ctl_t m_ctl = {};
    Serial *m_sdev = nullptr;

  public:
    bool init(Serial *dev);

    uint32_t irq_handler(ios_chunk_t *chunk);
};

#endif // APP_GPS_GNSSPARSER_H