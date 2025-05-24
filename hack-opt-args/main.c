#include <stdio.h>

#define LITER_ARGS(...) LCREATE ## __VA_OPT__(_FROM) (__VA_ARGS__)
#define LCREATE(...) {0}
#define LCREATE_FROM(...) {__VA_ARGS__}

typedef struct OptArgs {
    int arg1, arg2, arg3;
} OptArgs;

static void print_args_(OptArgs param)
{
    printf("  arg1 = %d\n  arg2 = %d\n  arg3 = %d\n", param.arg1, param.arg2, param.arg3);
}

#define print_args(...) print_args_((OptArgs)LITER_ARGS(__VA_ARGS__))
// #define print_args(...) print_args_((OptArgs){__VA_ARGS__})

#define label(msg) printf("\n" msg ":\n");

int main(void)
{
    label("sequential");
    print_args(1,2,3);

    label("named");
    print_args(.arg1 = 10, .arg3 = 30);

    label("null");
    print_args(0);

    label("mixed");
    print_args(1, .arg3 = 30);

    label("out of order");
    print_args(
        .arg2 = 20,
        .arg1 = 10,
        .arg3 = 30,
    );

    label("null2");
    print_args();

    // label("error");
    // print_args(.foo = 0);

    return 0;
}