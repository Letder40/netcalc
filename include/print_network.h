#ifndef PRINT_NETWORK_H 
#define PRINT_NETWORK_H 

#include <stdbool.h>
#include "netcalc.h"

typedef enum {
    NETWORK,
    SUBNET
} NetworkType;

void print_network(Network network, bool binary, NetworkType network_type);

#endif
