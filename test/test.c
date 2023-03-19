#include <onix/types.h>
#include <stdio.h>



typedef struct desciptor {
    unsigned short limit_low;//段界限0-15位
    unsigned int base_low:24; //基地址0-23位 16M
    unsigned char type : 4; //段类型
    unsigned char segment : 1; //1代表代码段或者数据段，0系统段
    unsigned char DPL : 2;  //描述符等级
    unsigned char present : 1;//1 内存中， 0 磁盘上
    unsigned char limit_high : 4;//段界限 16-19
    unsigned char available : 1;//
    unsigned char long_mode : 1;//64位扩展标志
    unsigned char big : 1;//32位还是16位
    unsigned char granularity : 1;//粒度4KB或1B
    unsigned char base_high;//基地址24-31位
} __attribute__((packed)) desciptor;

int main() 
{
    printf("size of u8 %d\n", sizeof(u8));
    printf("size of u16 %d\n", sizeof(u16));
    printf("size of u32 %d\n", sizeof(u32));
    printf("size of u64 %d\n", sizeof(u64));
    printf("size of desciptor %d\n", sizeof(desciptor));
    return 0;
}
