#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
util_assert(e) ((void)0)
#else
inline void util_assert(int e);
#endif

inline int min(int a, int b);
inline int max(int a, int b);



#ifdef __cplusplus
}
#endif
#endif