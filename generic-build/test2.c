#include <stdio.h>

#include <msg.h>
#include <sum.h>

int main(void)
{
    printf("%s\n", get_message());

    int a = 2;
    int b = 1;
    printf("%d + %d = %d\n", a, b, int_sum(a, b));
    return 0;
}