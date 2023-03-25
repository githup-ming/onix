#include <onix/onix.h>
#include <onix/io.h>

// CRT 地址寄存器 0x3d4
// CRT 数据寄存器 0x3d5
// CRT 光标位置 高位 0xe
// CRT 光标位置 低位 0xf

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5
#define CRT_POINT_HIGH 0xe
#define CRT_POINT_LOW 0xf



void kernel_init()
{
    outb(CRT_ADDR_REG, CRT_POINT_HIGH);
    u16 pos = inb(CRT_DATA_REG) << 8;
    outb(CRT_ADDR_REG, CRT_POINT_LOW);
    pos |= inb(CRT_DATA_REG);

    outb(CRT_ADDR_REG, CRT_POINT_HIGH);
    outb(CRT_DATA_REG, 0);
    outb(CRT_ADDR_REG, CRT_POINT_LOW);
    outb(CRT_DATA_REG, 79);

    return;

}