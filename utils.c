#include "utils.h"


inline void util_assert(int e)
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

inline int min(int a, int b)
{
    return (a > b ? b : a);
}

inline int max(int a, int b)
{
    return (a > b ? a : b);
}