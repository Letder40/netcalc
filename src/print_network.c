#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/netcalc.h"

#include "../ctable/include/ctable.h"

char* addr_to_binstr(uint32_t addr) {
    char* buf = malloc(sizeof(char) * 33);
    buf[32] = '\0';

    for (int i = 31; i >= 0; i--)
        buf[31 - i] = (addr & (1U << i)) ? '1' : '0'; 

    return buf;
}

ulong power(uint n, uint e) {
    if (!e) return 1;
    for (int i = 1; i < e; i++)
        n *= n;
    
    return n;
}

void octect_to_str(uint8_t addr_octect, char buf[4]) {
    int i = 0;
    for (; i<=2; i++)
        buf[i] = addr_octect / power(10, i) % 10 + '0';

    int right = i - 1;
    int left = 0;
    while (left < right) {
        if (buf[right] == '0') {
            buf[right--] = '\0';
            continue;
        }
        char tmp = buf[left];
        buf[left++] = buf[right];
        buf[right--] = tmp;
    }
}

char* addr_to_str(uint32_t addr) {
    char* buf = malloc(sizeof(char) * 16);
    uint buf_idx = 0;

    for (int i = 3; i >= 0; i--) {
        char octect_buf[4];
        uint octect_idx = 0;

        uint8_t octect = (addr >> i * 8) & 0xff;
        octect_to_str(octect, octect_buf);

        while (octect_buf[octect_idx] != '\0')
            buf[buf_idx++] = octect_buf[octect_idx++];

        buf[buf_idx++] = '.';
    }

    buf[--buf_idx] = '\0';
    return buf;
}

typedef enum {
    NETWORK,
    SUBNET
} NetworkType;

void print_network_binary(Network network, NetworkType network_type) {
    printf("%s Data [Binary]:\n", (network_type == NETWORK) ? "Network" : "Subnet");

    uint32_t broadcast_id = network.address | ~network.netmask;
    uint32_t network_id = network.address & network.netmask;

    char* addr_binstr = addr_to_binstr(network.address);
    char* netmask_binstr = addr_to_binstr(network.netmask);
    char* network_id_binstr = addr_to_binstr(network_id);
    char* broadcast_id_binstr = addr_to_binstr(broadcast_id);

    Table network_table_bin = (network_type == NETWORK)
        ? TABLE(
                ROW("Address", addr_binstr),
                ROW("Netmask", netmask_binstr),
                ROW("Network", netmask_binstr),
                ROW("Broadcast", broadcast_id_binstr)
        )
        : TABLE(
                ROW("Network", broadcast_id_binstr),
                ROW("Netmask", netmask_binstr),
                ROW("Broadcast", broadcast_id_binstr)
        );

    TableBuilder builder = table_build(network_table_bin);
    print_table(builder);

    free(addr_binstr);
    free(netmask_binstr);
    free(network_id_binstr);
    free(broadcast_id_binstr);
}

char* hosts_to_str(uint32_t hosts) {
    char* buf = malloc(11);
    if (!buf) {
        exit(EXIT_FAILURE);
    }

    int buf_idx = 0;
    if (hosts == 0) {
        buf[buf_idx++] = '0';
    } else {
        while (hosts > 0) {
            buf[buf_idx++] = (hosts % 10) + '0';
            hosts /= 10;
        }
    }

    buf[buf_idx] = '\0';

    for (int i = 0; i < buf_idx / 2; i++) {
        char temp = buf[i];
        buf[i] = buf[buf_idx - i - 1];
        buf[buf_idx - i - 1] = temp;
    }

    return buf;
}

void print_network(Network network, bool binary, NetworkType network_type) {
    printf("%s Data:\n", (network_type == NETWORK) ? "Network" : "\nSubnet");

    uint32_t broadcast_id = network.address | ~network.netmask;
    uint32_t network_id = network.address & network.netmask;
    uint32_t hosts = broadcast_id - network_id - 1;

    char* addr_str = addr_to_str(network.address);
    char* netmask_str = addr_to_str(network.netmask);
    char* network_id_str = addr_to_str(network_id);
    char* broadcast_id_str = addr_to_str(broadcast_id);
    char* hosts_str = hosts_to_str(hosts);

    Table network_table = (network_type == NETWORK)
        ? TABLE(
                ROW("Address", addr_str),
                ROW("Netmask", netmask_str),
                ROW("Network", network_id_str),
                ROW("Broadcast", broadcast_id_str)
        )
        : TABLE(
                ROW("Hosts", hosts_str),
                ROW("Netmask", netmask_str),
                ROW("Network", network_id_str),
                ROW("Broadcast", broadcast_id_str)
        );

    TableBuilder builder = table_build(network_table);
    print_table(builder);

    free(hosts_str);
    free(addr_str);
    free(netmask_str);
    free(network_id_str);
    free(broadcast_id_str);

    if (binary) print_network_binary(network, network_type);
}
