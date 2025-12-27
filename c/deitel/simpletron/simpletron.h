#include <stdint.h>

#define WORD_BYTES          2  // Instruction == one word
#define OPCODE_BYTES        1

#if WORD_BYTES == 1
typedef int8_t word_t;
typedef uint8_t uword_t;
#elif WORD_BYTES == 2
typedef int16_t word_t;
typedef uint16_t uword_t;
#elif WORD_BYTES == 4
typedef int32_t word_t;
typedef uint32_t uword_t;
#endif

#define OPERAND_BYTES       (WORD_BYTES - OPCODE_BYTES)
#define MEMORY_SIZE         (1 << (OPERAND_BYTES * 8))  // Memory address stored in operand

#define USER_INPUT_LENGTH   (2 + 1 + 8 * WORD_BYTES)  // max width for 0b00000000, 2 for 0b, 1 for \0
#define MAX_VALUE           (1 << (WORD_BYTES * 8))
#define STOP_VALUE          MAX_VALUE

/* Instructions */
#define NOP                 0x00
/* I/O operations */
#define READ                0x10  // Read value from keyboard and save to memory
#define WRITE               0x11  // Print value from memory
#define READSTR             0x12  // Read string from keyboard to memory
#define WRITESTR            0x13  // Print string from memory
/* Accumulator register operations */
#define LOAD                0x20  // Load from memory to accumulator
#define STORE               0x21  // Save accumulator to memory
/* Arithmetic operations */
#define ADD                 0x30  // Add to accumulator value from memory
#define SUBTRACT            0x31  // Subtract value from memory from accumulator
#define DIVIDE              0x32  // Divide value from memory into accumulator
#define MULTIPLY            0x33  // Multiply value from memory into accumulator
#define REMAINDER           0x34  // Divide value from memory into accumulator and save remainder
/* Transfer or control operations */
#define BRANCH              0x40  // Go to specified location
#define BRANCHNEG           0x41  // Go to specified location if accumulator is negative
#define BRANCHZERO          0x42  // Go to specified location if accumulator is zero
#define HALT                0x43  // Stop execution

#define MAX_COLS            0x10
#define SPACES              2
#define MEM_ADDR_WIDTH      4

#define ERRMSG              "\n*** Simpletron execution abnormally terminated ***\n"
#define SUCCESSMSG          "\n*** Simpletron execution terminated ***\n"

struct Simpletron {
    word_t memory[MEMORY_SIZE];    /* memory array */
    word_t instructionCounter;     /* current location in memory */
    word_t instructionRegister;    /* current instruction from memory */
    word_t operationCode;          /* current decoded operation */
    word_t operand;                /* current decoded operand */
    word_t accumulator;            /* accumulator register */
};

enum Status {STOP, SUCCESS, FAIL};


void simpletron_greet(void);
void soft_reset(struct Simpletron *);
void reset(struct Simpletron *);
bool check_value(int32_t);
enum Status user_input(word_t *);
enum Status execute_op(struct Simpletron *);
void print_state(struct Simpletron *);


inline void flush_input(void) {
    int strchar;
    while ((strchar = getchar()) != EOF && strchar != '\n') {}
}
