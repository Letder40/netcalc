#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "octect.h"

int** address_to_bits(char* arg){
    #define TRUE 1
    
    struct Octet octets[4];
    struct Octet *octet_ptr;
    
    for (int i=0; i<=3; i++){
        octets[i].counter = 0;
    }

    int octect_index = 0;
    int counter = 0;

    while(TRUE){
        octet_ptr = &octets[octect_index];

        if (arg[counter] == '\0'){
            break;
        }

        if (octet_ptr->counter > 3) {
            printf("[!] invalid address -> greeter value than 255\n");
            exit(1);
        }

        if (arg[counter] == '.'){
            octect_index ++;
            counter++;
            continue;
        }

        int digit = arg[counter] - '0';
        octet_ptr->digits[octet_ptr->counter] = digit;
        octet_ptr->counter++;
        counter++;

    }

    int digit_octs[4];

    for (int i=0; i<4; i++){
        if (octets[i].counter == 3){
            digit_octs[i] = octets[i].digits[0] * 100 + octets[i].digits[1] * 10 + octets[i].digits[2];
        }else if(octets[i].counter == 2){
            digit_octs[i] = octets[i].digits[0] * 10 + octets[i].digits[1]; 
        }else if(octets[i].counter == 1){
            digit_octs[i] = octets[i].digits[0];
        }

        if (digit_octs[i] > 255 || digit_octs[i] < 0) {
            printf("[!] invalid address -> a not valid value has been provided in the ip address\n");
            exit(1);
        }

    }

    // external layer allocation
    int **address_bits = malloc(sizeof(int*) * 4);

    // internal layer allocation
    for (int i = 0; i<4; i++){
        address_bits[i] = malloc(sizeof(int) * 8);
    }

    for (int byte_i = 0; byte_i < 4; byte_i++) {
        int to_convert = digit_octs[byte_i];
        for (int bit_i = 7; bit_i >= 0; bit_i--) {
            int bit = to_convert % 2;
            address_bits[byte_i][bit_i] = bit;
            to_convert /= 2;
        }
    }

    return address_bits;

}

int* decimal_of_ip(int bytes[4][8]){
    int *digits = malloc(sizeof(int) * 4);
    int counter = 0;
    for (int x=0; x<4; x++){
        digits[x] = 0;
        int counter = 0;
        for (int i=7; i>=0; i--){
            if (bytes[x][counter] == 0){
                counter++;
                continue;
            }
            digits[x] += pow(2, i); 
            counter++;
        }
    }
    return digits;
}
