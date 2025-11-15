#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = open("out.txt", O_CREAT | O_WRONLY, 0666);
    write(fd, "Hello world with low level file api\n", 36);
    close(fd);
    return 0;
}