#include <stdint.h>

#include "../include/netcalc.h"
#include "../include/args.h"
#include "../include/print_network.h"

uint8_t get_shitft(uint32_t subnets) { 
    uint8_t shift = 1;

    while ((shift << 1U) < subnets) {
        shift++;
    } 

    return shift;
}

void flsm(Args args) {
    Network subnet;
    subnet.netmask = args.network.netmask | args.network.netmask >> get_shitft(args.subnets);
    subnet.address = args.network.address & subnet.netmask;
    print_network(subnet, args.binary, SUBNET);

    for (int i = 1; i < args.subnets; i++) {
        subnet.address |= ~subnet.netmask;
        subnet.address++;
        print_network(subnet, args.binary, SUBNET);
    }
}
