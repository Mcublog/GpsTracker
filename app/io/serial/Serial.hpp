
#ifndef SERIAL_H
#define SERIAL_H

#include "app/io/serial_types.h"

class Serial
{
  private:
    ios_irq_handler_t m_irq_handler = nullptr;
  public:

    virtual bool Init(ios_ctl_t *ctl)
    {
        m_irq_handler = ctl->irq_handler;
        return true;
    };

    virtual bool Write(uint8_t *data, uint32_t size) { return false; };
    virtual bool Read(ios_message_t *message) { return false; };

    uint32_t IrqHandler(ios_chunk_t *chunk);
};

#endif // SERIAL_H