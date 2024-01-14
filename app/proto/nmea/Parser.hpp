
#ifndef APP_GPS_GNSSPARSER_H
#define APP_GPS_GNSSPARSER_H

#include <cstdbool>
#include <cstdint>

#include "app/interfaces/Serial.hpp"
#include "app/io/serial/types.h"
#include "libs/Ring-Buffer-0.1.1/ringbuffer.h"
#include "libs/lwgps-2.2.0/lwgps/src/include/lwgps/lwgps.h"

class GnssParser
{
  private:
    lwgps_t m_hgps;
    lwgps_t m_hgps_ready;

    ios_ctl_t m_ctl = {};
    Serial *m_sdev = nullptr;

    static constexpr uint32_t kRingBufferSize = 256;
    uint8_t m_ring_buffer_pool[kRingBufferSize];
    ring_buffer_t m_rb;

  public:
    bool init(Serial *dev);
    bool is_message_received(void);

    lwgps_t *read_message(void);

    uint32_t irq_handler(ios_chunk_t *chunk);
};

#endif // APP_GPS_GNSSPARSER_H