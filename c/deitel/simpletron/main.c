#include <stdio.h>
#include "simpletron.c"


int main(void) {
    struct Simpletron simpletron;
    enum Status status;
    input_data(&simpletron);
    do {
        status = execute_op(&simpletron);
    } while (status == EXEC);
    print_state(&simpletron);
    return 0;
}
