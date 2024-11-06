#ifndef LIBCMP_H
#define LIBCMP_H

#ifdef USE_STDINT
#include <stdint.h>
#define SIGNED_MAX INT32_MAX
#else
#define SIGNED_MAX 2147483647UL
#endif

// TODO: should those macros be turned into inline functions?
// example:
//   inline int gtiu(int a, unsigned int b)
//   {
//       return (0 < a) && (b < SIGNED_MAX) && (b < a);
//   }
#define gtiu(a,b) (!leiu((a), (b))) // a > b
#define geiu(a,b) (!ltiu((a), (b))) // a >= b
#define neiu(a,b) (!eqiu((a), (b))) // a != b

int ltiu(int a, unsigned int b); // a < b
int eqiu(int a, unsigned int b); // a == b
int leiu(int a, unsigned int b); // a <= b

inline int ltiu(int a, unsigned int b)
{
    return (a < 0) || (SIGNED_MAX < b) || (a < (int) b);
}

inline int eqiu(int a, unsigned int b)
{
    return (0 <= a) && (b <= SIGNED_MAX) && (a == (int) b);
}

inline int leiu(int a, unsigned int b)
{
    return (a <= 0) || (SIGNED_MAX <= b) || (a <= (int) b);
}



#endif // LIBCMP_H
