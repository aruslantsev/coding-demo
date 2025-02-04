#include <stdio.h>
#include <limits.h>

int main(void) {
    printf("%d %d %d %d %d %d\n", CHAR_BIT, SCHAR_MIN, SCHAR_MAX, CHAR_MIN, CHAR_MAX, UCHAR_MAX);
    printf("%d %d %d %d %d %d %ld %ld %ld\n", SHRT_MIN, SHRT_MAX, USHRT_MAX, INT_MIN, INT_MAX, UINT_MAX, LONG_MIN, LONG_MAX, ULONG_MAX);
    unsigned int i = -1;
    unsigned long int j = -1;
    printf("%u %u\n", i, j);
    return 0;
}
