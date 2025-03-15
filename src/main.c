#include <stdbool.h>
#include <stdint.h>

#include "../include/print_network.h"
#include "../include/args.h"
#include "../include/flsm.h"
#include "../include/vlsm.h"

int main(int argc, char* argv[]) {
    Args args = parse_args(argc, argv);
    switch (args.mode) {
        case NORMAL:
            print_network(args.network, args.binary, NETWORK);
            break;
        case FLSM:
            print_network(args.network, args.binary, NETWORK);
            flsm(args);
            break;
        case VLSM:
            print_network(args.network, args.binary, NETWORK);
            vlsm(args);
            break;
    }
}
