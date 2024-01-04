
#ifndef APP_PROCESS_WORKING_WDT_H
#define APP_PROCESS_WORKING_WDT_H

#include <cstdbool>
#include <cstdint>
#include <ctime>

class WorkingWdt
{
  private:
    static constexpr uint8_t kEventThreshold = 8;
    static constexpr uint8_t kSleepPeriodS = 15;

    uint8_t m_event_counter = 0;
    time_t m_last_mark = 0;

  public:
    void event_getting();
    void reset();

    bool is_expired();
    bool is_treshold();

    void save() const;
    void load();
};

#endif // APP_PROCESS_WORKING_WDT_H