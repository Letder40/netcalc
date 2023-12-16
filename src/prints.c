#include <stdio.h>
#include "clean_utils.c"

void print_network_range(int network_identifier[4], int broadcast_identifier[4]){
    network_identifier[3] += 1; 
    broadcast_identifier[3] -= 1;

    printf("[#] ");
    for (int i=0; i<4; i++){
        if (i == 3) {
            printf("%d", network_identifier[i]);
        }else{
            printf("%d.", network_identifier[i]);
        }
    }
    printf(" - ");
    for (int i=0; i<4; i++){
        if (i == 3) {
            printf("%d", broadcast_identifier[i]);
        }else{
            printf("%d.", broadcast_identifier[i]);
        }
    }
    printf("\n");
}

void print_binary_representations_ptr(char* header, int** bits){
    printf("%s", header);
        for (int x=0; x<4; x++){
            for (int i=0; i<8; i++){
            printf("%d", bits[x][i]);
            }
        }
        printf("\n");
    free_multiarray(bits);
}

void print_binary_representations(char* header, int bits[4][8]){
    printf("%s", header);
        for (int x=0; x<4; x++){
            for (int i=0; i<8; i++){
            printf("%d", bits[x][i]);
            }
        }
        printf("\n");
}

void print_identifiers(char* header, int* decimal_address){
    printf("%s", header);
        for (int i=0; i<4; i++){
            printf("%d", decimal_address[i]);
            if (i != 3) {
                printf(".");
            }
        }
        printf("\n");
}