#include "utils.h"


void util_assert(int e)
{
    if(e)
    {
#if defined(__arm__)
        __asm__("BKPT");
#else
        __builtin_trap();
#endif
        // TODO Execute error routine 


    }
}

int min(int a, int b)
{
    return (a > b ? b : a);
}

int max(int a, int b)
{
    return (a > b ? a : b);
}