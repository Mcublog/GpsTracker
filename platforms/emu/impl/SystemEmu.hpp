
#ifndef TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP
#define TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP

#include "app/interfaces/System.hpp"

class SystemEmu final : public System
{
  private:

  public:
    void init();
    void what(void);

    Serial *communication_serial();
    CobsParser *cobs_parser();
    GnssParser *gnss_parser();
};

#endif // TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP