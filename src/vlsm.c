#include <stdio.h>
#include <stdlib.h>

#include "../include/netcalc.h"
#include "../include/print_network.h"
#include "../include/args.h"

void get_vlsm(uint32_t hosts, Network* subnet) {
    if (hosts == 0) {
        printf("Number of hosts must be greater than zero.\n");
        exit(EXIT_FAILURE);
    }

    uint32_t mask = 0xffffffff;
    uint32_t shift = 1;

    while ((1U << shift) - 2 < hosts) {
        shift++;
    }
    
    subnet->netmask = mask << shift;
    uint32_t hosts_capacity = hosts_capacity = (~subnet->netmask) - 2;

    if (hosts > hosts_capacity) {
        printf("Hosts number [ %u ] exceeds the capacity of the network", hosts);
        exit(EXIT_FAILURE);
    }
}

int dsc_sort_f(const void *a, const void *b) {
    return (*(int *) b - *(int *) a);
}

void vlsm(Args args) {
    Network subnet;
    uint hosts[args.subnets];

    printf("\n[ VLSM Hosts ]\n");
    for (uint i = 0; i < args.subnets; i++) {
        uint host_n;
        printf("[?] Hosts in subnet %u: ", i);

        if (scanf("%d", &host_n) != 1) {
            printf("Invalid input! Please enter a valid number.\n");
            exit(EXIT_FAILURE);
        }

        hosts[i] = host_n;
    }

    qsort(hosts, args.subnets, sizeof(uint), dsc_sort_f);

    subnet.address = args.network.address;
    subnet.netmask = args.network.netmask;

    for (uint i = 0; i < args.subnets; i++) {
        get_vlsm(hosts[i], &subnet);
        print_network(subnet, args.binary, SUBNET);
        subnet.address |= ~subnet.netmask;
        subnet.address++;
    }
}

