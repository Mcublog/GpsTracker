
#ifndef SDEVICE_H
#define SDEVICE_H

#include <pthread.h>

#include "app/interfaces/Serial.hpp"

class SDevice : public Serial
{
  private:
    const char *m_portname = "/dev/ttyS11";
    void *(*m_read_thread)(void *) = nullptr;

    pthread_t m_thread_id;

  public:
    SDevice(const char *portname, void *(*read_thread)(void *));
    int m_io_stream;

    bool Init(ios_ctl_t *ctl);
    bool Helth(){return true;};
    bool Write(uint8_t *data, uint32_t size);
};

#endif // SDEVICE_H