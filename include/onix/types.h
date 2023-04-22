#ifndef __TYPES_H__
#define __TYPES_H__

#include <onix/onix.h>

#define EOF -1 //END OF FILE
#define EOS '\0'  //字符串结束符
#define NULL ((void *)0) //空指针
#ifndef __cplusplus //兼容c++
#define bool _Bool
#define true 1
#define false 0
#endif

#define _packed __attribute__((packed)) //用于定义特殊的结构
#define _ofp __attribute__((optimize("omit-frame-pointer"))) // 用于省略函数的栈帧

typedef unsigned int size_t;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char *va_list;

typedef void *handler_t; // 中断处理函数
typedef u32 time_t;


#endif