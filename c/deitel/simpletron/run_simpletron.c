#include <stdio.h>
#include "simpletron.h"


int main(int argc, char *argv[]) {
    struct Simpletron simpletron;
    enum Status status;
    if (argc == 1) {
        input_sml(&simpletron);
    } else if (argc == 2) {
        reset(&simpletron);
        read_file_sml(&simpletron, argv[1]);
        print_state(&simpletron);
    } else {
        puts("Usage: ");
        printf("\t%s\t\t\tto enter program from keyboard\n", argv[0]);
        printf("\t%s FILENAME\tto read program from file\n", argv[0]);
        return 0;
    }

    do {
        status = execute_op(&simpletron);
    } while (status == SUCCESS);
    print_state(&simpletron);
    return 0;
}
