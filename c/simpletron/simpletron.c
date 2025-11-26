#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OPERAND_LENGTH      2
#define INSTRUCTION_LENGTH  2
#define MEMORY_SIZE         100

#define READ                10
#define WRITE               11
#define LOAD                20
#define STORE               21
#define ADD                 30
#define SUBTRACT            31
#define DIVIDE              32
#define MULTIPLY            33
#define BRANCH              40
#define BRANCHNEG           41
#define BRANCHZERO          42
#define HALT                43

#define MAX_COLS            10


struct Simpletron {
    long    memory[MEMORY_SIZE];    /* memory array */
    long    instructionCounter;     /* current location in memory */
    long    instructionRegister;    /* current instruction from memory */
    long    operationCode;          /* current decoded operation */
    long    operand;                /* current decoded operand */
    long    accumulator;            /* accumulator register */
};

enum Status {STOP, FAIL, EXEC};

void print_state(struct Simpletron *);


void simpletron_greet(void) {
    puts(
        "*** Welcome to Simpletron! ***\n"
        "*** Please enter your program one instruction ***\n"
        "*** (or data word) at a time. I will type the ***\n"
        "*** location number and a question mark (?). ***\n"
        "*** You then type the word for that location. ***\n"
        "*** Type the sentinel -99999 to stop entering ***\n"
        "*** your program. ***\n"
    );
}


void soft_reset(struct Simpletron *simpletron) {
    simpletron->accumulator = 0;
    simpletron->instructionCounter = 0;
    simpletron->instructionRegister = 0;
    simpletron->operationCode = 0;
    simpletron->operand = 0;
}


void reset(struct Simpletron *simpletron) {
    soft_reset(simpletron);
    for (size_t counter = 0; counter < MEMORY_SIZE; simpletron->memory[counter++] = 0);
}


enum Status execute_op(struct Simpletron *simpletron) {
    simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter++];
    int sign = simpletron->instructionRegister >= 0? 1: -1;
    simpletron->operationCode = (sign * simpletron->instructionRegister) / 100;
    simpletron->operand = sign * ((sign * simpletron->instructionRegister) % 100);

    switch (simpletron->operationCode) {
        case READ:
            printf("%s", "<- ");
            long user_input;
            scanf("%ld", &user_input);
            if (abs(user_input) < pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH))
                simpletron->memory[simpletron->operand] = user_input;
            else {
                puts("Invalid input. Should be in range -9999..9999");
                return FAIL;
            }
            break;
        case WRITE:
            printf("-> %+05ld\n", simpletron->memory[simpletron->operand]);
            break;
        case LOAD:
            simpletron->accumulator = simpletron->memory[simpletron->operand];
            break;
        case STORE:
            simpletron->memory[simpletron->operand] = simpletron->accumulator;
            break;
        case ADD:
            simpletron->accumulator += simpletron->memory[simpletron->operand];
            break;
        case SUBTRACT:
            simpletron->accumulator -= simpletron->memory[simpletron->operand];
            break;
        case DIVIDE:
            if (simpletron->memory[simpletron->operand] != 0)
                simpletron->accumulator /= simpletron->memory[simpletron->operand];
            else
                printf("Division by zero at %ld\n", simpletron->instructionCounter - 1);
                return FAIL;
            break;
        case MULTIPLY:
            simpletron->accumulator *= simpletron->memory[simpletron->operand];
            break;
        case BRANCH:
            simpletron->instructionCounter = simpletron->operand;
            break;
        case BRANCHNEG:
            if (simpletron->accumulator < 0) {
                simpletron->instructionCounter = simpletron->operand;
            }
            break;
        case BRANCHZERO:
            if (simpletron->accumulator == 0) {
                simpletron->instructionCounter = simpletron->operand;
            }
            break;
        case HALT:
            puts("\n*** Simpletron execution terminated ***\n");
            return STOP;
            break;
        default:
            printf("Invalid instruction %ld at %ld\n", simpletron->instructionRegister, simpletron->instructionCounter - 1);
            return FAIL;
    }
    if (abs(simpletron->accumulator) < pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH))
        return EXEC;
    else {
        printf("Accumulator value %ld is not in range -9999..9999\n", simpletron->accumulator);
        return FAIL;
    }
}


void input_data(struct Simpletron *simpletron) {
    long input;

    simpletron_greet();

    reset(simpletron);
    do {
        printf("%04ld ? ", simpletron->instructionCounter);
        scanf("%ld", &input);
        if (abs(input) < pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH)) {
            simpletron->memory[simpletron->instructionCounter++] = input;
        } else if (input != -99999) {
            puts("Invalid input");
        }
    } while (input != -99999);
    soft_reset(simpletron);
}


void print_state(struct Simpletron *simpletron) {
    printf("accumulator:\t\t%+05ld\n", simpletron->accumulator);
    printf("instructionCounter:\t%5ld\n", simpletron->instructionCounter);
    printf("instructionRegister:\t%+05ld\n", simpletron->instructionRegister);
    printf("operationCode:\t\t%5ld\n", simpletron->operationCode);
    printf("operand:\t\t%5ld\n", simpletron->operand);

    puts("\nMEMORY:");
    printf("%s", "    ");
    for (size_t counter = 0; counter < MAX_COLS; counter++)
        printf("%7ld", counter);
    puts("");

    for (size_t counter = 0; counter < MEMORY_SIZE; counter++) {
        if (counter % MAX_COLS == 0) {
            if (counter > 0)
                puts("");
            printf("%4ld", counter / MAX_COLS);
        }

        printf("  %+05ld", simpletron->memory[counter]);
    }
    puts("\n");
}
