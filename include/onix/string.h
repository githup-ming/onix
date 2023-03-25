#ifndef __ONIX_STRING_H__
#define __ONIX_STRING_H__

#include <onix/types.h>


int8 *strcpy(int8 *dest, const int8 *src);
int8 *strcat(int8 *dest, const int8 *src);
size_t strlen(const int8 *str);
int32 strcmp(const int8 *lhs, const int8 *rhs);
int8 *strchr(const int8 *str, int32 ch);
int8 *strrchr(const int8 *str, int32 ch);

int32 memcmp(const void *lhs, const void *rhs, size_t count);
void *memset(void *dest, int32 ch, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memchr(const void *str, int32 ch, size_t count);


#endif