#include <fcntl.h>
 #include <unistd.h>

int main(void)
{
    int fd = open("out.txt", O_CREAT | O_WRONLY, 0664);
    write(fd, "Hello file descriptor\n", 22);
    close(fd);
    return 0;
}