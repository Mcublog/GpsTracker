
#ifndef DATA_PRINT_H
#define DATA_PRINT_H

#include "app/interfaces/System.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    void dprint_wakeup_cause(const wakeup_cause_t *cause);

#ifdef __cplusplus
}
#endif

#endif // DATA_PRINT_H