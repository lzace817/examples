.global _start

_start:
    mov  $1           , %rax        # write
    mov  $1           , %rdi        # stdout
    mov  $message     , %rsi        # pointer to str
    mov  $message_len , %rdx        # length of str
    syscall


    mov $60, %rax                   # exit
    mov $0 , %rdi                   # exit code
    syscall                         #

message:
    .ascii "Hello world from ASM\n"
.equ message_len, .-message


