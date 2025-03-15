#ifndef NETCALC_H
#define NETCALC_H

#include <stdint.h>

typedef struct Network {
    uint32_t address;
    uint32_t netmask;
} Network;

#endif
