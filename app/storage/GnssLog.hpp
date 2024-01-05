
#ifndef APP_STORAGE_GNSS_LOG_HPP
#define APP_STORAGE_GNSS_LOG_HPP

#include <stdint.h>

#include "app/storage/Log.hpp"

class GnssLog final : public Log
{
  public:
    void init() override;
    void save_to_backup();
};

#endif // APP_STORAGE_GNSS_LOG_HPP