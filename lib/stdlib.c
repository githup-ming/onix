#include <onix/stdlib.h>



void delay(u32 count)
{
    while (count--);
}
void hang()
{
    while(true);
}
//bcd码转换为整数
u8 bcd_to_bin(u8 value)
{
    return (value & 0xf) + (value >> 4) * 10;
}
//整数转换为bcd码
u8 bin_to_bcd(u8 value)
{
    return (value / 10) * 0x10 + (value % 10);
}
//计算num 分成size 的数量
u32 div_round_up(u32 num, u32 size)
{
    return (num + size - 1) / size;
}
