#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipconv.c"

#define MAX_BUF 100

int power2 (int times) {
    int number = 2;

    if (times == 0) 
        return 1;

    for (int i=1; i<times; i++) {
        number *= 2;
    }
    return number;
}

int* get_number_address(int hosts, int times) {
    int result = power2(times);
    if (hosts > result - 2) {
        return get_number_address(hosts, ++times);
    }else {
        int* addresses_and_prefix = malloc(sizeof(int) * 2);
        memset(addresses_and_prefix, 0, sizeof(int) * 2);

        addresses_and_prefix[0] = result;
        addresses_and_prefix[1] = times;
        return addresses_and_prefix;
    }
}

struct subnet {
    char network_id[32];
    char broadcast_id[32];
    char netmask[32];
};

void vlsm(char* ip_address, char* netmask, int netmask_prefix) {
    int number_subnets;
    char buff[MAX_BUF] = {0};
    printf("[?] number of subnets: ");
    fgets(buff, MAX_BUF, stdin);
    number_subnets = atoi(buff);
    memset(buff, 0, MAX_BUF);
    
    if (!number_subnets) {
        printf("\n[!] Not a valid number of subnets\n");
        exit(1);
    }

    int subnets_addresses = 0; 
    int subnets_total_hosts[number_subnets]; 
    int subnets_requested_hosts[number_subnets];
    int subnets_prefix[number_subnets]; 
    int max_addresses = power2(32 - netmask_prefix);

    for (int i = 0; i<number_subnets; i++){
        printf("[?] number of hosts for subnet %i: ", i + 1);
        fgets(buff, MAX_BUF, stdin);
        int number_hosts = atoi(buff);
        
        if (!number_hosts) {
            printf("\n[!] Not a valid number of hosts\n");
            exit(1);
        }
        
        subnets_requested_hosts[i] = number_hosts;

        int* addresses_and_prefix = get_number_address(number_hosts, 0);
        subnets_addresses += addresses_and_prefix[0];
        subnets_total_hosts[i] = addresses_and_prefix[0] - 2;
        subnets_prefix[i] = 32 - addresses_and_prefix[1];
        free(addresses_and_prefix);
        
        if (subnets_addresses > max_addresses) {
            printf("\n[!] You have exceeded the max number of addresses in a /%i\n", netmask_prefix);
            exit(1);
        }
    }

    for (int i = 0; i<number_subnets - 1; i++) {
        for (int j=0; j<number_subnets - i - 1; j++) {
            if (subnets_prefix[j] > subnets_prefix[j + 1]) {
                int temp = subnets_prefix[j];
                subnets_prefix[j] = subnets_prefix[j + 1];
                subnets_prefix[j + 1] = temp;

                temp = subnets_requested_hosts[j];
                subnets_requested_hosts[j] = subnets_requested_hosts[j+1];
                subnets_requested_hosts[j + 1] = temp;

                temp = subnets_total_hosts[j];
                subnets_total_hosts[j] = subnets_total_hosts[j + 1];
                subnets_total_hosts[j + 1] = temp;
            }
        }
    }

    for (int i=0; i<32; i++)
        ip_address[i] = ip_address[i] & netmask[i];

    char* subnet = ip_address;
    for (int i=0; i<32; i++) subnet[i] = ip_address[i];
    struct subnet subnets[number_subnets];

    for (int i = 0; i<number_subnets; i++) {
        char subnet_mask[32] = {0};
        for (int j=0; j<subnets_prefix[i]; j++) {
            subnet_mask[j] = 1;
        }

        for (int j=0; j<32; j++) {
            subnets[i].network_id[j] = subnet[j] & subnet_mask[j];
            subnets[i].broadcast_id[j] = subnet[j] | (subnet_mask[j] ^ 1);
            subnets[i].netmask[j] = subnet_mask[j];
        }

        for (int j=0; j<32; j++) subnet[j] = subnets[i].broadcast_id[j];
        unsigned int carry = 1;
        for (int j = 31; j >= 0; j--) {
            unsigned int sum = subnet[j] + carry;
            subnet[j] = sum % 2;
            carry = sum / 2;
        }       

        printf("\n[ Subnet %i ]\n", i + 1);
        printf("[#] Requested Hosts   %i\n", subnets_requested_hosts[i]);
        printf("[#] Total Host        %i\n", subnets_total_hosts[i]);
        printf("[#] Network ID        %s/%i\n", to_decimalstr(subnets[i].network_id), subnets_prefix[i]);
        printf("[#] Broadcast ID      %s/%i\n", to_decimalstr(subnets[i].broadcast_id), subnets_prefix[i]);
        printf("[#] Subnet Mask       %s\n", to_decimalstr(subnets[i].netmask));        
    }
}