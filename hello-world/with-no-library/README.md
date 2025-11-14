# hello world with syscall

> [!WARNING]
> TODO: find a way to compile it statically with no libc

# references

- [musl](https://musl.libc.org/):
    - [`arch/x86_64/syscall_arch.h`](https://github.com/kraj/musl/blob/master/arch/x86_64/syscall_arch.h)
    - [`src/internal/syscall.h`](https://github.com/kraj/musl/blob/master/src/internal/syscall.h)
    - [`src/internal/syscall_ret.c`](https://github.com/kraj/musl/blob/master/src/internal/syscall_ret.c)