#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../include/netcalc.h"
#include "../include/args.h"

uint address_from_str(char* network) {
    uint address = 0;

    int shift = 24;
    char* octect_str = strtok(network, ".");
    while (octect_str != NULL && shift >= 0) {
        address |= atoi(octect_str) << shift;
        octect_str = strtok(NULL, ".");
        shift -= 8;
    }

    return address;
}

typedef struct {
    Network network;
    bool failure;
} NetworkResult;

NetworkResult network_from_str(char* network_str, char* netmask_str) {
    Network network = {
        address_from_str(network_str),
        address_from_str(netmask_str)
    };

    NetworkResult result = {
        network,
        false
    };

    return result;
}

Args default_args() {
    Args args;
    args.binary = false;
    args.mode = NORMAL;

    return args;
}

void help() {
    printf("Usage: \n"
           "netcalc [options] [MODE] {address netmask}\n"
           "MODES: \n"
           "-f | --flsm: [Subnets Number]: fixed leght subnet mask mode, the number of subnets is required\n"
           "-v | --vlsm: variable leght subnet mask mode\n"
           "if no mode is specified only range calculation will be enabled\n"
           "OTHERS:\n"
           "-h | --help: show this help\n"
           "-b | --binary: show binary data\n"
           "EXAMPLES:\n"
           "netcalc 192.168.1.0 255.255.255.0\n"
           "netcalc 10.15.100.5 255.224.0.0\n"
           "netcalc -f 4 192.168.1.0 255.255.255.0\n"
           "netcalc -v 192.168.1.0 255.255.255.0\n");
}

uint32_t subnets_number_fromstr(char* str) {
    char* endptr;
    uint subnets_number = strtol(str, &endptr, 10);
    if (*endptr != '\0') {
        printf("A valid number of subnets is required\n");
        exit(EXIT_FAILURE);
    }
    return subnets_number; 
}

Args parse_args(int argc, char** argv) {
    Args args = default_args();
    bool network_defined = false;
    bool mode_defined = false;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            help();
            exit(EXIT_SUCCESS);
        } else if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--binary")) {
            args.binary = true;

        } else if (!mode_defined && i + 1 < argc &&
                   (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--vlsm"))) {
            args.mode = VLSM;
            mode_defined = true;
            args.subnets = subnets_number_fromstr(argv[i + 1]);
            i++;

        } else if (!mode_defined && i + 1 < argc &&
                   (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--flsm"))) {
            args.mode = FLSM;
            mode_defined = true;
            args.subnets = subnets_number_fromstr(argv[i + 1]);
            i++;

        } else if (!network_defined && i + 1 < argc) {
            NetworkResult result = network_from_str(argv[i], argv[i + 1]);
            i++;

            if (result.failure) {
                printf("Invalid address or netmask\n");
                help();
                exit(EXIT_FAILURE);
            }

            args.network = result.network;
            network_defined = true;

        } else {
            printf("Invalid address or netmask\n");
            help();
            exit(EXIT_FAILURE);
        }
    }

    return args;
}
