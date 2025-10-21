// begin of x86_64 magic //////////////////////////////////////////////////////

static inline long __syscall3(long n, long a1, long a2, long a3)
{
    unsigned long ret;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3) : "rcx", "r11", "memory");
    return ret;
}

int errno = 0;
static long write(int fd, const void* buf, unsigned long count)
{
    unsigned long r = __syscall3(1, fd, (long)buf, count);
    if (r > -4096UL) { // -4096UL casts to: 18_446_744_073_709_547_520
        errno = -r;
        return -1;
    }
    return r;
}

// end of x86_64 magic ////////////////////////////////////////////////////////

int main(void)
{
    int r = write(1, "hello world\n", 12);
    if (r == -1) {
        return errno;
    }

    return 0;
}