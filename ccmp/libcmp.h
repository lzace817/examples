#ifndef LIBCMP_H
#define LIBCMP_H

#ifdef USE_STDINT
#include <stdint.h>
#define INT_MAX INT32_MAX
#else
#define INT_MAX 2147483647
#endif

// #define leiu(a,b) (ltiu((a), (b)) || eqiu((a), (b)))
#define gtiu(a,b) (!leiu((a), (b)))
#define geiu(a,b) (!ltiu((a), (b)))
#define neiu(a,b) (!eqiu((a), (b)))

int ltiu(int a, unsigned int b);
int eqiu(int a, unsigned int b);
int leiu(int a, unsigned int b);

// int gtiu(int a, unsigned int b);
// int geiu(int a, unsigned int b);
// int neiu(int a, unsigned int b);

inline int ltiu(int a, unsigned int b)
{
    // if (a < 0) return 1;
    // if (b > INT_MAX) return 1;
    // return a < (int) b;
    return (a < 0) || (INT_MAX < b) || (a < (int) b);
}

inline int eqiu(int a, unsigned int b)
{
    // if (a < 0) return 0;
    // if (b > INT_MAX) return 0;
    // return (a == (int) b);
    return (0 <= a) && (b <= INT_MAX) && (a == (int) b);
}

inline int leiu(int a, unsigned int b)
{
    // if (a < 0) return 1;
    // if (b > INT_MAX) return 1;
    // return a <= (int) b;
    return (a < 0) || (INT_MAX < b) || (a <= (int) b);
}

#endif // LIBCMP_H

#ifdef LIBCMP_STB_IMPLEMENTATION
#if 0
// TODO: should all function bellow be turned into macros?
// If we use it, we don't have typechecking
// we will, probably have better performance with inline funcions
int leiu(int a, unsigned int b)
{
    return (ltiu(a, b) || eqiu(a, b));
}

int gtiu(int a, unsigned int b)
{
    return !leiu(a, b);
}

int geiu(int a, unsigned int b)
{
    return !ltiu(a, b);
}

int neiu(int a, unsigned int b)
{
    return !eqiu(a, b);
}
#endif
#endif //LIBCMP_STB_IMPLEMENTATION

