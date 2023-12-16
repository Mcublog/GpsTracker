
#ifndef SDEVICE_H
#define SDEVICE_H

#include "app/io/serial/Serial.hpp"

class SDevice : public Serial
{
  private:
    const char *DEFAULT_SERIAL_PORT = "/dev/ttyS11";

  public:
    bool Init(ios_ctl_t *ctl);
    bool Write(uint8_t *data, uint32_t size);
};

#endif // SDEVICE_H