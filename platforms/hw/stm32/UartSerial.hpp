#ifndef PLATFORMS_HW_STM32_UARTSERIAL_HPP
#define PLATFORMS_HW_STM32_UARTSERIAL_HPP

#include "app/interfaces/Serial.hpp"

class UartSerial final : public Serial
{
  private:
    /* data */
  public:
    bool init(ios_ctl_t *ctl) override;
    bool helth();
    bool write(uint8_t *data, uint32_t size);
};

#endif // PLATFORMS_HW_STM32_UARTSERIAL_HPP