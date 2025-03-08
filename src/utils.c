#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char* to_binarystr(char* ip) {
    char* str_ip = malloc(40);
    memset(str_ip, 0, 40);
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
    memset(str_ip, 0, 16);
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
