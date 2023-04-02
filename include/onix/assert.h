#ifndef __ONIX_ASSERT_H__
#define __ONIX_ASSERT_H__

#include <onix/types.h>

void assertion_failure(char *exp, char *file, char *base, int line);

#define assert(exp) \
    if (exp)        \
        ;           \
    else            \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)

void panic(const char *fmt, ...);



#endif