/*
 * Copyright © 2014 Kosma Moczek <kosma@cloudyourcar.com>
 * This program is free software. It comes without any warranty, to the extent
 * permitted by applicable law. You can redistribute it and/or modify it under
 * the terms of the Do What The Fuck You Want To Public License, Version 2, as
 * published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef FLASHSIM_H
#define FLASHSIM_H

#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct flashsim;

    struct flashsim *flashsim_open(const char *name, int size, int sector_size);
    void flashsim_close(struct flashsim *sim);

    void flashsim_sector_erase(struct flashsim *sim, uint32_t addr);
    void flashsim_read(struct flashsim *sim, uint32_t addr, uint8_t *buf, int len);
    void flashsim_program(struct flashsim *sim, uint32_t addr, const uint8_t *buf, int len);

#ifdef __cplusplus
}
#endif

#endif // FLASHSIM_H

/* vim: set ts=4 sw=4 et: */
