#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "prints.c"
#include "convertions.c"

int main(int argv ,char** args) {
    int** netmask_bits;
    int** ip_bits;

    if (argv == 1){
        char* input = malloc(sizeof(char) * 15);
        printf("[ IPv4 addresses ]\n");
        
        printf("[?] ip address            ");
        fgets(input, 15, stdin);
        input[strcspn(input, "\n")] = '\0';
        ip_bits = address_to_bits(input);

        printf("[?] network mask          ");
        fgets(input, 15, stdin);
        input[strcspn(input, "\n")] = '\0';
        netmask_bits = address_to_bits(input);
        free(input);

    }else if (argv == 3) {
        ip_bits = address_to_bits(args[1]);
        netmask_bits = address_to_bits(args[2]);  
        printf("[ IPv4 addresses ]\n");
        printf("[#] ip address            %s\n", args[1]);
        printf("[#] network mask          %s\n", args[2]);      
    }else{
        printf("Syntx error, syntax: <binary> ip_adress network_mask");
    }

    // Checking if netmask is a valid one
    int bit_change = 1;
    int bit_change_counter = 0;
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
            if (bit_change == 1 && netmask_bits[x][i] == 0) {
                bit_change = 0;
                bit_change_counter ++;
            }else if(bit_change == 0 && netmask_bits[x][i] == 1){
                bit_change=1;
                bit_change_counter++;
            }
        }
    }

    if (bit_change_counter > 1){
        printf("[!] Invalid network mask\n");
        return 1;
    }

    // And operation: network mask and ip address to get network identifier

    int network_identifier[4][8];
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
            network_identifier[x][i] = netmask_bits[x][i] && ip_bits[x][i];
        }
    }

    // Or operation: negated network mask or ip address to get broadcast identifier

    int broadcast_identifier[4][8];
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
            broadcast_identifier[x][i] = !netmask_bits[x][i] || ip_bits[x][i];
        }
    }

    // printing binary representation

    printf("\n[ Binary representation ] \n");

    print_binary_representations_ptr("[#] ip address            ", ip_bits);
    print_binary_representations_ptr("[#] network mask          ", netmask_bits);
    print_binary_representations("[#] network identifier    ", network_identifier);
    print_binary_representations("[#] broadcast identifier  ", broadcast_identifier);

    // Getting decimal representation of network identifier, and broadcast identifier

    int* decimal_network_identifier = decimal_of_ip(network_identifier);
    int* decimal_broadcast_identifier = decimal_of_ip(broadcast_identifier);

    // Printing decimal representation

    printf("\n[ Decimal representation ] \n");

    print_identifiers("[#] network identifier    ", decimal_network_identifier);
    print_identifiers("[#] broadcast identifier  ", decimal_broadcast_identifier);

    printf("\n[ Netwrok range ] \n");
    print_network_range(decimal_network_identifier, decimal_broadcast_identifier);

    return 0;

}