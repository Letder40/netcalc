#ifndef ARGS_H
#define ARGS_H

#include <sys/types.h>
#include <stdbool.h> 
#include "netcalc.h"

typedef enum {
    NORMAL,
    FLSM,
    VLSM,
} NETCALC_MODE;

typedef struct {
    Network network;
    NETCALC_MODE mode;
    uint subnets;
    bool binary;
} Args;

Args parse_args(int argc, char** argv);
#endif
