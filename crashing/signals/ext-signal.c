#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    printf("%s\n", __FILE__);
    pid_t pid = getpid();
    printf("pid: %u\n", pid);
    fflush(stdout);

    getc(stdin);// example: on new terminal type `kill -SIGTERM <pid-number>`

    printf("done\n");
    return 0;
}