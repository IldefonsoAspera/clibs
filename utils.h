#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
util_assert(e) ((void)0)
#else
void util_assert(int e);
#endif

#define _CONCAT_2(a, b) a##b
#define CONCAT_2(a, b) _CONCAT_2(a, b)

#define _CONCAT_3(a, b, c) a##b##c
#define CONCAT_3(a, b, c) _CONCAT_3(a, b, c)


int min(int a, int b);
int max(int a, int b);



#ifdef __cplusplus
}
#endif
#endif