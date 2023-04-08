#ifndef __ONIX_GLOBAL_H__
#define __ONIX_GLOBAL_H__

#include <onix/types.h>

#define GDT_SIZE 128

typedef struct descriptor_t {
    u16 limit_low;//段界限0-15位
    u32 base_low:24; //基地址0-23位 16M
    u8 type : 4; //段类型
    u8 segment : 1; //1代表代码段或者数据段，0系统段
    u8 DPL : 2;  //描述符等级
    u8 present : 1;//1 内存中， 0 磁盘上
    u8 limit_high : 4;//段界限 16-19
    u8 available : 1;//
    u8 long_mode : 1;//64位扩展标志
    u8 big : 1;//32位还是16位
    u8 granularity : 1;//粒度4KB或1B
    u8 base_high;//基地址24-31位
} _packed descriptor_t;


typedef struct selector_t
{
    u8 RPL : 2;
    u8 TI : 1;
    u16 index : 13;
} selector_t;



typedef struct pointer_t
{
    u16 limit;
    u32 base;
}_packed  pointer_t;


void gdt_init();


#endif