#include <stdio.h>

int main(void)
{
    printf("%s\n", __FILE__);
    fflush(stdout);
    *((char *)0) = 0;
    printf("done\n");
    return 0;
}