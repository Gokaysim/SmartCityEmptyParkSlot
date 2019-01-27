#define DEBUG 1
#define INT_MAX 4294967295
#ifndef NULL
#define NULL 0
#endif
#if DEBUG

#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)

#else

#define PRINTF(...)

#endif
