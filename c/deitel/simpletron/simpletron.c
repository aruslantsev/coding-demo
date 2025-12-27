#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "simpletron.h"
// TODO: strings processing: optimize, use one byte to save one symbol


void simpletron_greet(void) {
    printf(
        "*** Welcome to Simpletron! ***\n"
        "*** Please enter your program one instruction ***\n"
        "*** (or data word) at a time. I will type the ***\n"
        "*** location number and a question mark (?). ***\n"
        "*** You then type the hexadecimal word for that  ***\n"
        "*** location. Type the sentinel %X to stop entering ***\n"
        "*** your program. ***\n\n",
        STOP_VALUE
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


bool check_value(const int32_t value) {
    if (labs(value) < MAX_VALUE)
        return true;
    return false;
}


enum Status user_input(word_t *value) {
    char s[USER_INPUT_LENGTH];
    fgets(s, USER_INPUT_LENGTH, stdin);
    const word_t parsed_input = (word_t) strtol(s, nullptr, 0);
    if (check_value(parsed_input)) {
        *value = parsed_input;
        return SUCCESS;
    }
    return FAIL;
}


enum Status execute_op(struct Simpletron *simpletron) {
    if (simpletron->instructionCounter < 0 || simpletron->instructionCounter >= MEMORY_SIZE) {
        printf("*** instructionCounter is not in range 0..%d ***\n", MEMORY_SIZE);
        puts(ERRMSG);
        return FAIL;
    }

    /* Decode instruction */
    simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter++];
    if (simpletron->instructionRegister < 0) {
        printf(
                "*** Invalid instruction %d at %d ***\n",
                simpletron->instructionRegister, simpletron->instructionCounter - 1
            );
        return FAIL;
    }
    simpletron->operationCode = (uword_t) simpletron->instructionRegister >> (OPERAND_BYTES * 8);
    simpletron->operand = (uword_t) simpletron->instructionRegister & ((1 << OPERAND_BYTES * 8) - 1);

    /* Execute instruction */
    size_t memptr = simpletron->operand;
    int strchar;

    switch (simpletron->operationCode) {
        case NOP:
            break;
        case READ:
            printf("%s", "<- ");
            if (user_input(&simpletron->memory[simpletron->operand]) != SUCCESS) {
                printf("*** Invalid input. Should be in decimal range %d..%d ***\n", -MAX_VALUE + 1, MAX_VALUE - 1);
                return FAIL;
            }
            break;
        case WRITE:
            printf("-> %+0*d\n", WORD_BYTES * 2 + 1, simpletron->memory[simpletron->operand]);
            break;
        case READSTR:
            printf("%s", "<- ");
            while ((strchar = getchar()) != '\n') {
                simpletron->memory[memptr++] = (uword_t) strchar;
            }
            simpletron->memory[memptr] = 0;
            break;
        case WRITESTR:
            printf("%s", "-> ");
            while ((strchar = (int) simpletron->memory[memptr++]) != 0)
                putchar(strchar);
            putchar('\n');
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
            if (simpletron->memory[simpletron->operand] != 0) {
                simpletron->accumulator /= simpletron->memory[simpletron->operand];
            } else {
                printf("*** Attempt to divide by zero at %d ***\n", simpletron->instructionCounter - 1);
                puts(ERRMSG);
                return FAIL;
            }
            break;
        case MULTIPLY:
            simpletron->accumulator *= simpletron->memory[simpletron->operand];
            break;
        case REMAINDER:
            if (simpletron->memory[simpletron->operand] != 0) {
                simpletron->accumulator %= simpletron->memory[simpletron->operand];
            } else {
                printf("*** Attempt to divide by zero at %d ***\n", simpletron->instructionCounter - 1);
                puts(ERRMSG);
                return FAIL;
            }
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
            puts(SUCCESSMSG);
            return STOP;
        default:
            printf(
                "*** Invalid instruction %X at %X ***\n",
                simpletron->instructionRegister, simpletron->instructionCounter - 1
            );
            puts(ERRMSG);
            return FAIL;
    }
    if (check_value(simpletron->accumulator))
        return SUCCESS;
    printf(
    "*** Accumulator value %d is not in range %d..%d ***\n",
        simpletron->accumulator, MAX_VALUE, -MAX_VALUE
    );
    puts(ERRMSG);
    return FAIL;
}


void print_state(struct Simpletron *simpletron) {
    printf("accumulator:\t\t%0*X\n", WORD_BYTES * 2, simpletron->accumulator);
    printf("instructionCounter:\t%*X\n", WORD_BYTES * 2, simpletron->instructionCounter);
    printf("instructionRegister:\t%0*X\n", WORD_BYTES * 2, simpletron->instructionRegister);
    printf("operationCode:\t\t%*X\n", WORD_BYTES * 2, simpletron->operationCode);
    printf("operand:\t\t%*X\n", WORD_BYTES * 2, simpletron->operand);
    puts("\nMEMORY:");
    printf("%*s", MEM_ADDR_WIDTH, "");
    for (size_t counter = 0; counter < MAX_COLS; counter++)
        printf("%*lX", SPACES + WORD_BYTES * 2, counter);
    puts("");

    for (size_t counter = 0; counter < MEMORY_SIZE; counter++) {
        if (counter % MAX_COLS == 0) {
            if (counter > 0)
                puts("");
            printf("%*lX", MEM_ADDR_WIDTH, counter / MAX_COLS);
        }
        printf("%*s%0*X", SPACES, "", WORD_BYTES * 2, (uword_t) simpletron->memory[counter]);
    }
    puts("\n");
}
