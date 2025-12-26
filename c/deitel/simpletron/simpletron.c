#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define OPERAND_LENGTH      2
#define INSTRUCTION_LENGTH  2
#define MEMORY_SIZE         100L

#define MAX_VALUE           ((long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH) - 1)

/* Instructions */
#define NOP                 00
/* I/O operations */
#define READ                10  // Read value from keyboard and save to memory
#define WRITE               11  // Print value from memory
#define READSTR             12  // Read string from keyboard to memory
#define WRITESTR            13  // Print string from memory
/* Accumulator register operations */
#define LOAD                20  // Load from memory to accumulator
#define STORE               21  // Save accumulator to memory
/* Arithmetic operations */
#define ADD                 30  // Add to accumulator value from memory
#define SUBTRACT            31  // Subtract value from memory from accumulator
#define DIVIDE              32  // Divide value from memory into accumulator
#define MULTIPLY            33  // Multiply value from memory into accumulator
#define REMAINDER           34  // Divide value from memory into accumulator and save remainder
/* Transfer or control operations */
#define BRANCH              40  // Go to specified location
#define BRANCHNEG           41  // Go to specified location if accumulator is negative
#define BRANCHZERO          42  // Go to specified location if accumulator is zero
#define HALT                43  // Stop execution

#define MAX_COLS            10
#define SPACES              2
#define MEM_ADDR_WIDTH      4

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


bool check_value(const long value) {
    if (labs(value) < MAX_VALUE) {
        return true;
    } else {
        return false;
    }
}

enum Status execute_op(struct Simpletron *simpletron) {
    if (simpletron->instructionCounter < 0 || simpletron->instructionCounter >= MEMORY_SIZE) {
        printf("*** instructionCounter is not in range 0..%ld ***\n", MEMORY_SIZE);
        puts(ERRMSG);
        return FAIL;
    }
    simpletron->instructionRegister = simpletron->memory[simpletron->instructionCounter++];
    const int sign = simpletron->instructionRegister >= 0? 1: -1;
    simpletron->operationCode = labs(simpletron->instructionRegister) / (long) pow(10, OPERAND_LENGTH);
    simpletron->operand = sign * labs(simpletron->instructionRegister % (long) pow(10, OPERAND_LENGTH));

    size_t memptr = simpletron->operand;
    int strchar;

    switch (simpletron->operationCode) {
        case NOP:
            break;
        case READ:
            printf("%s", "<- ");
            long user_input;
            scanf("%ld", &user_input);
            // TODO: strtol()
            if (check_value(user_input)) {
                simpletron->memory[simpletron->operand] = user_input;
            } else {
                printf("*** Invalid input. Should be in range %ld..%ld ***\n", MAX_VALUE, -MAX_VALUE);
                return FAIL;
            }
            break;
        case WRITE:
            printf("-> %+0*ld\n", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->memory[simpletron->operand]);
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
            if (simpletron->accumulator != 0) {
                simpletron->accumulator = simpletron->memory[simpletron->operand] / simpletron->accumulator;
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
    if (check_value(simpletron->accumulator))
        return EXEC;
    else {
        printf(
            "*** Accumulator value %ld is not in range %ld..%ld ***\n",
            simpletron->accumulator, MAX_VALUE, -MAX_VALUE
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
        printf("%0*ld ? ", INSTRUCTION_LENGTH + OPERAND_LENGTH, simpletron->instructionCounter);
        scanf("%ld", &input);
        if (check_value(input)) {
            simpletron->memory[simpletron->instructionCounter++] = input;
        } else if (input != (- (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH + 1) + 1)) {
            puts("Invalid input");
        }
    } while (input != (- (long) pow(10, OPERAND_LENGTH + INSTRUCTION_LENGTH + 1) + 1));
    soft_reset(simpletron);
}


void print_state(struct Simpletron *simpletron) {
    printf("accumulator:\t\t%+0*ld\n", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->accumulator);
    printf("instructionCounter:\t%*ld\n", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->instructionCounter);
    printf("instructionRegister:\t%+0*ld\n", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->instructionRegister);
    printf("operationCode:\t\t%*ld\n", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->operationCode);
    printf("operand:\t\t%*ld\n", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->operand);
    puts("\nMEMORY:");
    printf("%*s", MEM_ADDR_WIDTH, "");
    for (size_t counter = 0; counter < MAX_COLS; counter++)
        printf("%*ld", SPACES + INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, counter);
    puts("");

    for (size_t counter = 0; counter < MEMORY_SIZE; counter++) {
        if (counter % MAX_COLS == 0) {
            if (counter > 0)
                puts("");
            printf("%*ld", MEM_ADDR_WIDTH, counter / MAX_COLS);
        }

        printf("%*s%+0*ld", SPACES, "", INSTRUCTION_LENGTH + OPERAND_LENGTH + 1, simpletron->memory[counter]);
    }
    puts("\n");
}
