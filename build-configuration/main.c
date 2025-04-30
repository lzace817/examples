#include <stdio.h>

#ifdef CONFIG_PLAIN
#define HELLO_MSG "hello world"
#endif // CONFIG_PLAIN

#ifdef CONFIG_FANCY
#define HELLO_MSG "Hello, World!"
#endif // CONFIG_FANCY

int main(void)
{
    printf("%s\n", HELLO_MSG);
    return 0;
}