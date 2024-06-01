#include <stdio.h>
#include <assert.h>

int main(void)
{
    printf("%s\n", __FILE__);
    fflush(stdout);
    assert(0);
    printf("done\n");
    return 0;
}