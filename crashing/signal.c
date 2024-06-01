#include <stdio.h>
#include <signal.h>

int main(void)
{
    printf("%s\n", __FILE__);
    fflush(stdout);
    raise(SIGINT);
    printf("done\n");
    return 0;
}