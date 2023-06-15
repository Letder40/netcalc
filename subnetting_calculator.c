#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int** get_ip(){
    int digit;
    int octet_index = 0;

    struct Octet {
        int digits[4];
        int counter;
    };

    struct Octet octets[4];
    struct Octet *octet_ptr;

    for (int i=0; i<=3; i++){
        octets[i].counter = 0;
    }

    while (1) {
        
        octet_ptr = &octets[octet_index];
        int counter = octet_ptr->counter;

        digit = getchar();

        if (digit == '\n') {
            break;
        }

        if (digit == '.') {
            octet_index++;
            continue;
        }

        digit = digit - '0';

        if (octet_ptr->counter > 2 || octet_index > 3) {
            printf("[!] invalid IP address -> greeter value than 255\n");
            exit(1);
        }

        if (digit < 0 || digit > 9) {
            printf("[!] invalid IP address -> not a valid digit\n");
            exit(1);
        }
        
        octet_ptr->digits[counter] = digit;
        octet_ptr->counter++;
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
            printf("[!] invalid IP address -> a not valid value has been provided in the ip address\n");
            exit(1);
        }
        
    }
    
    // multidim array.

    // external layer allocation
    int **ip_bits = malloc(sizeof(int*) * 4);

    // internal layer allocation
    for (int i = 0; i<4; i++){
        ip_bits[i] = malloc(sizeof(int) * 8);
    }


    int counter;

    for (int byte_i = 0; byte_i < 4; byte_i++) {
        int to_convert = digit_octs[byte_i];
        for (int bit_i = 7; bit_i >= 0; bit_i--) {
            int bit = to_convert % 2;
            ip_bits[byte_i][bit_i] = bit;
            to_convert /= 2;
        }
    }

    return ip_bits;
}

int main() {

    printf("[?] ip address -> ");
    int** ip_bits = get_ip();
    printf("[?] netmask -> ");
    int** netmask_bits = get_ip();

    // Checking if netmask is a valid one
    int state = 1;
    int state_change = 0;
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
            if (state == 1 && netmask_bits[x][i] == 0) {
                state = 0;
                state_change ++;
            }else if(state == 0 && netmask_bits[x][i] == 1){
                state=1;
                state_change++;
            }
        }
    }
    if (state_change > 1){
        printf("[!] Invalid network mask\n");
        return 1;
    }

    int network_identifier[4][8];
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
            network_identifier[x][i] = netmask_bits[x][i] && ip_bits[x][i];
        }
    }

    int broadcast_identifier[4][8];
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
            broadcast_identifier[x][i] = !netmask_bits[x][i] || ip_bits[x][i];
        }
    }

    printf("\n[ Binary representation ] \n");

    printf("[#] ip address            ");
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
        printf("%d", ip_bits[x][i]);
        }
    }
    printf("\n");
    free(ip_bits);

    printf("[#] network mask          ");
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
        printf("%d", netmask_bits[x][i]);  
        }
    }
    printf("\n");
    free(netmask_bits);

    printf("[#] network identifier    ");
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
        printf("%d", network_identifier[x][i]);
        }
    }
    printf("\n");

    printf("[#] broadcast identifier  ");
    for (int x=0; x<4; x++){
        for (int i=0; i<8; i++){
        printf("%d", broadcast_identifier[x][i]);
        }
    }
    printf("\n");

    int decimal_network_identifier[4], decimal_broadcast_identifier[4];
    for (int x=0; x<4; x++){
        decimal_network_identifier[x] = 0;
        int counter = 0;
        for (int i=7; i>=0; i--){
            if (network_identifier[x][counter] == 0){
                counter++;
                continue;
            }
            decimal_network_identifier[x] += pow(2, i); 
            counter++;
        }
    }

    for (int x=0; x<4; x++){
        decimal_broadcast_identifier[x] = 0;
        int counter = 0;
        for (int i=7; i>=0; i--){
            if (broadcast_identifier[x][counter] == 0){
                counter++;
                continue;
            }
            decimal_broadcast_identifier[x] += pow(2, i); 
            counter++;
        }
    }

    printf("\n[ Decimal representation ] \n");

    printf("[#] network identifier    ");
    for (int i=0; i<4; i++){
        printf("%d", decimal_network_identifier[i]);
        if (i != 3) {
            printf(".");
        }
    }
    printf("\n");

    printf("[#] broadcast identifier  ");
    for (int i=0; i<4; i++){
        printf("%d", decimal_broadcast_identifier[i]);
        if (i != 3) {
            printf(".");
        }
    }
    printf("\n");

    return 0;

}