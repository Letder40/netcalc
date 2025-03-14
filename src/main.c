#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../ctable/include/ctable.h"

typedef struct Network {
    uint32_t address;
    uint32_t netmask;
} Network;

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

void print_network(Network network) {
    char buf[33] = {0};

    printf("Network Data: \n");
    char* addr = addr_to_str(network.address);
    char* netmask = addr_to_str(network.netmask);
    char* network_id = addr_to_str(network.address & network.netmask);
    char* broadcast_id = addr_to_str(network.address | ~network.netmask);

    Table network_table = TABLE(
        ROW("Address", addr),
        ROW("Netmask", netmask),
        ROW("Network", network_id),
        ROW("Broadcast", broadcast_id)
    );

    TableBuilder builder = table_build(network_table);

    free(addr);
    free(netmask);

    print_table(builder);

    char* addr_bin = addr_to_binstr(network.address);
    char* netmask_bin = addr_to_binstr(network.address);
    char* network_bin_id = addr_to_binstr(network.address & network.netmask);
    char* broadcast_bin_id = addr_to_binstr(network.address | ~network.netmask);

    printf("\nNetwork Binary Data: \n");
    Table network_table_bin = TABLE(
        ROW("Address", addr_bin),
        ROW("Netmask", netmask_bin),
        ROW("Network", network_bin_id),
        ROW("Broadcast", broadcast_bin_id)
    );

    builder = table_build(network_table_bin);

    free(addr);
    free(netmask);

    print_table(builder);
}


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

Network network_from_strparts(char* network_str, char* netmask_str) {
    Network network = {
        address_from_str(network_str),
        address_from_str(netmask_str)
    };

    return network;
}

int main(int argc, char* argv[]) {
    Network network = network_from_strparts(argv[1], argv[2]);
    print_network(network);
}
