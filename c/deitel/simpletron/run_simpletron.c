#include <stdio.h>
#include <stdlib.h>

#include "simpletron.h"


void input_data(struct Simpletron *simpletron) {
    int32_t input;
    char s[USER_INPUT_LENGTH];

    reset(simpletron);
    simpletron_greet();
    do {
        printf("%0*x ? ", OPERAND_BYTES * 2, simpletron->instructionCounter);
        fgets(s, USER_INPUT_LENGTH, stdin);
        input = strtol(s, nullptr, 16);

        if (check_value(input)) {
            simpletron->memory[simpletron->instructionCounter++] = (word_t) input;
        } else if (input != STOP_VALUE) {
            puts("Invalid input");
        }
    } while (input != STOP_VALUE);
    soft_reset(simpletron);
}


void read_file(struct Simpletron *simpletron, char *filename) {
    reset(simpletron);
    FILE *file;
    int32_t header = 0;
    size_t result = 0;

    file = fopen(filename, "rb");
    if (file == NULL) {
        puts("Error opening file");
        exit(1);
    }
    result = fread(&header, sizeof(header), 1, file);
    if (result == 0) {
        puts("Error reading file");
        exit(1);
    }
    if (header == 0b11110101) { // binary file
        puts("Reading binary file");
        while (!feof(file)) {
            result = fread(&simpletron->memory[simpletron->instructionCounter++], sizeof(word_t), 1, file);
            if (result == 0) puts("Error reading file");
        }
    } else {
        fclose(file);
        puts("Reading text file");
        file = fopen(filename, "r");
        int32_t input;
        char s[USER_INPUT_LENGTH];
        while (!feof(file)) {
            if (fgets(s, USER_INPUT_LENGTH, file) != NULL) {
                input = strtol(s, nullptr, 16);
                if (check_value(input)) {
                    simpletron->memory[simpletron->instructionCounter++] = (word_t) input;
                } else puts("Invalid input");
            }
        }
    }
    fclose(file);
    soft_reset(simpletron);
}


int main(int argc, char *argv[]) {
    struct Simpletron simpletron;
    enum Status status;
    if (argc == 1) {
        input_data(&simpletron);
    } else if (argc == 2) {
        reset(&simpletron);
        read_file(&simpletron, argv[1]);
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
