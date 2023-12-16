#include <stdlib.h>

void free_multiarray(int** array){
    for (int i = 0; i<4; i++){
        free(array[i]);
    }
    free(array);
}