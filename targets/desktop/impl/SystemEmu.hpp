
#ifndef TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP
#define TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP

#include "app/interfaces/System.hpp"

class SystemEmu final : public System
{
  private:

  public:

    void what(void) override;
    Serial *get_serial_device(void) override;
};

#endif // TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP