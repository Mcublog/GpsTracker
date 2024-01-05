
#ifndef APP_GPS_GNSSPARSER_H
#define APP_GPS_GNSSPARSER_H

#include <cstdint>
#include <cstdbool>

#include "app/interfaces/Serial.hpp"
#include "app/io/serial/types.h"
#include "libs/lwgps-2.2.0/lwgps/src/include/lwgps/lwgps.h"

class GnssParser
{
  private:
    lwgps_t m_hgps;
    lwgps_t m_hgps_ready;
    bool m_msg_ready = false;

    ios_ctl_t m_ctl = {};
    Serial *m_sdev = nullptr;

  public:
    bool init(Serial *dev);
    bool is_message_received(void) const;

    lwgps_t *read_message(void);

    uint32_t irq_handler(ios_chunk_t *chunk);
};

#endif // APP_GPS_GNSSPARSER_H