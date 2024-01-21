
#ifndef TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP
#define TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP

#include "app/interfaces/System.hpp"

class SystemEmu final : public System
{
  private:
    sys_performance_t m_perfomance = sys_performance_t::LOW_POWER;

  public:
    void init();
    void what(void);

    Serial *communication_serial();
    CobsParser *cobs_parser();
    GnssParser *gnss_parser();

    void performance_set(sys_performance_t perf) override {m_perfomance = perf;};
    sys_performance_t performance_get() const override { return m_perfomance;};

    void peripheral_reinit() override {};
};

#endif // TARGETS_DESKTOP_IMPL_SYSTEM_EMU_HPP