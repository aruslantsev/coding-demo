#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define OPERAND_LENGTH      2
#define INSTRUCTION_LENGTH  2
#define MEMORY_SIZE         100L

#define NOP                 00
#define READ                10
#define WRITE               11
#define READSTR             12
#define WRITESTR            13
#define LOAD                20
#define STORE               21
#define ADD                 30
#define SUBTRACT            31
#define DIVIDE              32
#define MULTIPLY            33
#define REMAINDER           34
#define BRANCH              40
#define BRANCHNEG           41
#define BRANCHZERO          42
#define HALT                43

#define MAX_COLS            10

#define ERRMSG              "\n*** Simpletron execution abnormally terminated ***\n"
#define SUCCESSMSG          "\n*** Simpletron execution terminated ***\n"


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
    printf(
        "*** Welcome to Simpletron! ***\n"
        "*** Please enter your program one instruction ***\n"
        "*** (or data word) at a time. I will type the ***\n"
        "*** location number and a question mark (?). ***\n"
        "*** You then type the word for that location. ***\n"
        "*** Type the sentinel %ld to stop entering ***\n"
        "*** your program. ***\n\n",
         (- (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH + 1) + 1)
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
    if (simpletron->instructionCounter < 0 || simpletron->instructionCounter >= MEMORY_SIZE) {
        printf("*** instructionCounter is not in range 0..%ld ***\n", MEMORY_SIZE);
        puts(ERRMSG);
        return FAIL;
    }
    simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter++];
    int sign = simpletron->instructionRegister >= 0? 1: -1;
    simpletron->operationCode = (sign * simpletron->instructionRegister) / (long) pow(10, OPERAND_LENGTH);
    simpletron->operand = sign * ((sign * simpletron->instructionRegister) % (long) pow(10, OPERAND_LENGTH));

    size_t memptr = simpletron->operand;
    int strchar;

    switch (simpletron->operationCode) {
        case NOP:
            break;
        case READ:
            printf("%s", "<- ");
            long user_input;
            scanf("%ld", &user_input);
            if (abs(user_input) < pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH)) {
                simpletron->memory[simpletron->operand] = user_input;
            } else {
                printf(
                    "*** Invalid input. Should be in range %ld..%ld ***\n",
                    (long) -pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH) + 1,
                    (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH) - 1
                );
                return FAIL;
            }
            break;
        case WRITE:
            printf("-> %+05ld\n", simpletron->memory[simpletron->operand]);
            break;
        case READSTR:
            while ((strchar = getchar()) != EOF && strchar != '\n');
            printf("%s", "<- ");
            while ((strchar = getchar()) != '\n') {
                simpletron->memory[memptr++] = strchar;
            }
            simpletron->memory[memptr] = 0;
            break;
        case WRITESTR:
            size_t memptr = simpletron->operand;
            printf("%s", "-> ");
            while ((strchar = simpletron->memory[memptr++]) != 0)
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
                printf(
                    "*** Attempt to divide by zero at %ld ***\n",
                    simpletron->instructionCounter - 1
                );
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
                printf(
                    "*** Attempt to divide by zero at %ld ***\n",
                    simpletron->instructionCounter - 1
                );
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
            break;
        default:
            printf(
                "*** Invalid instruction %ld at %ld ***\n",
                simpletron->instructionRegister, simpletron->instructionCounter - 1
            );
            puts(ERRMSG);
            return FAIL;
    }
    if (abs(simpletron->accumulator) < pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH))
        return EXEC;
    else {
        printf(
            "*** Accumulator value %ld is not in range %ld..%ld ***\n",
            simpletron->accumulator,
            (long) -pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH) + 1,
            (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH) - 1
        );
        puts(ERRMSG);
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
        } else if (input != (- (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH + 1) + 1)) {
            puts("Invalid input");
        }
    } while (input != (- (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH + 1) + 1));
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
