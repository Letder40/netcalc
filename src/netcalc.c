#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* to_binarystr(char* ip) {
    char* str_ip = malloc(40);
    int str_ip_index = 0;
    for (int i = 0; i<32; i++) {
        str_ip[str_ip_index++] = ip[i] + '0';
        if (i == 7 || i == 15 || i == 23) 
            str_ip[str_ip_index++] = '.';
    }

    return str_ip;
}

char* to_decimalstr(char* ip) {
    char* str_ip = malloc(16);
    int str_ip_index = 0;
    for (int i = 0; i<4; i++) {
        int power = 1;
        int decimal_ip = 0;

        for (int j = 8 - 1; j>=0; j--) {
            if (ip[j + i*8] == 1) {
                decimal_ip += power;
            }
            power *= 2;
        }

        char temp[5];
        sprintf(temp, "%d.", decimal_ip);
        strcat(str_ip, temp);
    }
    str_ip[strlen(str_ip) - 1] = '\0';
    return str_ip;
}

void help (FILE* fd) {
    fprintf(fd, "Usage: sncalc [ip-addres/mask-prefix]\n");
}

int main(int argc, char** argv) {
    if (argc != 2) {
        help(stderr);
        return 1;
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
        if(i != 0) {
        }
        for (int j = 0; j<7; j++) {
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