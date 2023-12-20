
#ifndef SDEVICE_H
#define SDEVICE_H

#include "app/interfaces/Serial.hpp"

class SDevice : public Serial
{
  private:
    const char *m_portname = "/dev/ttyS11";

  public:
    SDevice(const char *portname);

    bool Init(ios_ctl_t *ctl);
    bool Write(uint8_t *data, uint32_t size);
};

#endif // SDEVICE_H