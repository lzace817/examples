#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "libcmp.h"

#define ptest(stmt, expect) printf(#stmt " : %s -- %s\n", \
        ((stmt))?"true":"false", ((stmt)==(expect))?"\033[1;32mOK\033[0m":"\033[1;31mfail\033[0m")

void test(int i, unsigned int u)
{
    printf("TESTING FOR\n");
    printf("    u = %u\n", u);
    printf("    i = %d\n", i);

// #ifdef TEST_C_DEFAULT
    printf("======= c default ========\n");
    ptest(i  < u, true);
    ptest(i == u, false);
    ptest(i <= u, true);

    ptest(i > u, false);
    ptest(i != u, true);
    ptest(i >= u, false);
// #endif
    // libcmp
    printf("\n======== libcmp ==========\n");
    ptest(ltiu(i, u), true);
    ptest(eqiu(i, u), false);
    ptest(leiu(i, u), true);

    ptest(gtiu(i, u), false);
    ptest(neiu(i, u), true);
    ptest(geiu(i, u), false);

    printf("\n");
}

int main(void)
{
    // i should be smaller than u on all tests bellow.
    test(8, 200);
    test(50, 3000000000);
    test(-2000000000, 3000000000);
    test(-1, UINT32_MAX);
    test(-3, 7);
    return 0;
}