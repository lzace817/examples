#include <stdio.h>

int main(void) {
    //TODO(proto): test for different types
    int a,b;
    a = 1000000000;
    b = 2000000000;
    printf("----- before + ------\n");
    printf("%i + %i = %i\n", a, b, a+b);
    printf("----- after + -------\n");
    return 0;
}