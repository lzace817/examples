#define retval 0

#define HAVE_NO_ARGS(...) CHECK(__VA_ARGS__ __VA_OPT__(,) 0, 0, 0, 1)
#define CHECK(a1, a2, a3, a4, ...) a4

probe = (HAVE_NO_ARGS());
// probe = (HAVE_NO_ARGS(a,b));
probe = (HAVE_NO_ARGS(t));
