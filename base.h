#define DEBUG 1
#define NULL 0
#if DEBUG

#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)

#else

#define PRINTF(...)

#endif
