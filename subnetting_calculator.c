#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void print_network_range(int decimal_network_identifier[4], int decimal_broadcast_identifier[4]){
    for (int i=0; i<4; i++){
            if ( i == 3){
                printf("%d", decimal_network_identifier[i] + 1);
            }else{
                printf("%d", decimal_network_identifier[i]);
                printf(".");
            }
        }
        printf(" - ");
        for (int i=0; i<4; i++){
            if ( i == 3){
                printf("%d", decimal_broadcast_identifier[i] - 1);
            }else{
                printf("%d", decimal_network_identifier[i]);
                printf(".");
            }
        }
        printf("\n");
}

void free_multiarray(int** array){
    for (int i = 0; i<4; i++){
        free(array[i]);
    }
    free(array);
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

int** get_ip(){
    int digit;
    int octet_index = 0;

    // octects declaration

    struct Octet {
        int digits[4];
        int counter;
    };

    struct Octet octets[4];
    struct Octet *octet_ptr;

    for (int i=0; i<=3; i++){
        octets[i].counter = 0;
    }
    //

    while (1) {
        
        octet_ptr = &octets[octet_index];
        int counter = octet_ptr->counter;

        digit = getchar();

        // finish when digit = 10 ('\n') decimal representation in ascii
        if (digit == '\n') {
            break;
        }

        // breaking ip address to another octect when the digit is equal to 46 ('.') decimal representation in ascii

        if (digit == '.') {
            octet_index++;
            continue;
        }

        // Getting real int 

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
    print_binary_representations("[#] network identifier      ", network_identifier);
    print_binary_representations("[#] broadcast identifier     ", broadcast_identifier);

    // Getting decimal representation of network identifier, and broadcast identifier

    int* decimal_network_identifier = decimal_of_ip(network_identifier);
    int* decimal_broadcast_identifier = decimal_of_ip(broadcast_identifier);

    // Printing decimal representation

    printf("\n[ Decimal representation ] \n");

    print_identifiers("[#] network identifier   ", decimal_network_identifier);
    print_identifiers("[#] broadcast identifier  ", decimal_broadcast_identifier);

    // printing network range
    
    printf("\n[ Network valid range] \n[#] ");

    print_network_range(decimal_network_identifier, decimal_broadcast_identifier);

    return 0;

}