#include <stdio.h>

enum operation
{
    ADD = 1,
    SUBTRACT = 2,
    MULTIPLY = 4
};

struct BinaryOp{
    int n1;
    int n2;
    enum operation op;
};


int main (void)
{
    struct BinaryOp mulOp = {.n1 = 6, .n2 = 8, .op = MULTIPLY};
    printf("Operands: %d and %d. Operation: %d\n", mulOp.n1, mulOp.n2, mulOp.op);
    // Operands: 6 and 8. Operation: 4
    switch (mulOp.op) {
        case ADD:
            printf("%d\n", mulOp.n1 + mulOp.n2);
            break;
        case SUBTRACT:
            printf("%d\n", mulOp.n1 - mulOp.n2);
            break;
        case MULTIPLY:
            printf("%d\n", mulOp.n1 * mulOp.n2);
            break;
        default:
            printf("Unknown op\n");
            break;
    }

    enum days {Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};
    for(int i = Monday;i <= Sunday; i++)
        printf("%d, ", i);
    printf("\n");
    return 0;
}
