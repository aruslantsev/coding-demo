#include <stdio.h>
#include <string.h>



// Union definition
union A {
    int i;
    float f;
    char s[20];
};

int main() {

    union A a;

    // Storing an integer
    a.i = 10;
    printf("data.i = %d\n", a.i);

    // Storing a float
    a.f = 220.5;
    printf("data.f = %.2f\n", a.f);
    printf("data.i = %d\n", a.i);  // Wrong value!

    // Storing a string
    strcpy(a.s, "GfG");
    printf("data.s = %s\n", a.s);

    return 0;
}
