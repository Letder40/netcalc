#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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

struct Octet {
    int digits[4];
    int counter;
};

int** get_ip(){
    int digit;
    int octet_index = 0;

    // octects declaration

    struct Octet octets[4];
    struct Octet *octet_ptr;

    for (int i=0; i<=3; i++){
        octets[i].counter = 0;
    }
    //

    while (1) {
        
        octet_ptr = &octets[octet_index];

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

        if (digit < 0 || digit > 9) {
            printf("[!] invalid address -> not a valid digit\n");
            exit(1);
        }
        
        octet_ptr->digits[octet_ptr->counter] = digit;
        octet_ptr->counter++;

        if (octet_ptr->counter > 3) {
            printf("[!] invalid address -> greeter value than 255\n");
            exit(1);
        }
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
    
    // multidim array.

    // external layer allocation
    int **address_bits = malloc(sizeof(int*) * 4);

    // internal layer allocation
    for (int i = 0; i<4; i++){
        address_bits[i] = malloc(sizeof(int) * 8);
    }


    int counter;

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

     // multidim array.

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