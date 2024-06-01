#include <stdio.h>

int main(void)
{
    printf("%s\n", __FILE__);
    fflush(stdout);
    int d = 0;
    int i = 1/d;
    printf("done\n");
    return 0;
}