#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subnetting.c"

void help (FILE* fd) {
    fprintf(fd, "Usage:\nnetcalc {ip-addres/mask-prefix} [s]\n\nOPTIONS\n  -s subnetting mode\n");
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        help(stderr);
        return 1;
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        help(stdout);
        return 0;
    }

    if (strstr(argv[1], "/") == NULL || strlen(argv[1]) - 1 == strcspn(argv[1], "/") ){
        help(stderr);
        return 1;
    }

    int subnetting = 0;
    if (argc == 3){
        if (strcmp(argv[2], "-s")) {
            help(stderr);
            return 1;
        }
        printf("[#] Subnetting mode\n");
        subnetting = 1;
    }

    char* string_ip = strtok(argv[1], "/");
    char* netmask_prefix = strtok(NULL, "/");
    char* string_ip_cpy = argv[1];

    int prefix = atoi(netmask_prefix);
    if (prefix > 32 || prefix <= 0) {
        fprintf(stderr, "not a valid netmask");
        return 1;
    }

    unsigned char numbers_ip[4] = {0};
    int index = 0;
    while (*string_ip) {
        if (index > 3) {
            fprintf(stderr, "[!] Not a valid IPv4 address");
            return 1;
        }
        if (isdigit(*string_ip)) {
            numbers_ip[index] *= 10;
            numbers_ip[index] += *string_ip - '0';            
        } else {
            index++;
        }
        string_ip++;
    }
    string_ip = string_ip_cpy;

    char ip_address[32] = {0};
    for (int i = 0; i<4; i++ ){
        for (int j = 0; j<8; j++) {
            int index = j + (8*i); 
            ip_address[index] = (numbers_ip[i] >> (7 - j)) & 1;
        }
    }

    char netmask[32] = {0};
    for (int i = 0; i<32; i++) {
        if (i < prefix) {
            netmask[i] = 1;
        } else {
            netmask[i] = 0;
        }
    }

    if (subnetting == 1) {
        vlsm(ip_address, netmask, prefix);
        return 0;
    }

    char network_id[32] = {0};
    char broadcast_id[32] = {0};
    for (int i = 0; i<32; i++) {
        network_id[i] = ip_address[i] & netmask[i];  
        broadcast_id[i] = ip_address[i] | (netmask[i] ^ 1);  
    }


    printf("\n\
[ Decimal representation ]\n\
[#] IPv4 Address   %s\n\
[#] Network Mask   %s\n\
[#] Network ID     %s\n\
[#] Broadcast ID   %s\n\n\
\
[ Binary representation]\n\
[#] IPv4 Address   %s\n\
[#] Network Mask   %s\n\
[#] Network ID     %s\n\
[#] Broadcast ID   %s\n\n\
", string_ip, to_decimalstr(netmask), to_decimalstr(network_id), to_decimalstr(broadcast_id), to_binarystr(ip_address), to_binarystr(netmask), to_binarystr(network_id), to_binarystr(broadcast_id)\
    );
}