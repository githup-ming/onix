#include <onix/rtc.h>
#include <onix/interrupt.h>
#include <onix/assert.h>
#include <onix/debug.h>
#include <onix/io.h>
#include <onix/stdlib.h>
#include <onix/time.h>

#define CMOS_ADDR 0x70 // CMOS 地址寄存器
#define CMOS_DATA 0x71 // CMOS 数据寄存器

#define CMOS_NMI 0x80

#define CMOS_A 0x0a
#define CMOS_B 0x0b
#define CMOS_C 0x0c
#define CMOS_D 0x0d

u8 cmos_read(u8 addr)
{
    outb(CMOS_ADDR, CMOS_NMI | addr);
    return inb(CMOS_DATA);
}

void cmos_write(u8 addr, u8 value)
{
    outb(CMOS_ADDR, CMOS_NMI | addr);
    outb(CMOS_DATA, value);
}

static u32 counter;

// 设置闹钟
void set_alarm(u32 secs)
{
    LOGK("beeping after %d seconds\n", secs);

    tm time;
    time_read(&time);

    u8 sec = secs % 60;
    secs /= 60;
    u8 min = secs % 60;
    secs /= 60;
    u32 hour = secs;

    time.tm_sec += sec;
    if (time.tm_sec >= 60)
    {
        time.tm_sec %= 60;
        time.tm_min += 1;
    }

    time.tm_min += min;
    if (time.tm_min >= 60)
    {
        time.tm_min %= 60;
        time.tm_hour += 1;
    }

    time.tm_hour += hour;
    if (time.tm_hour >= 24)
    {
        time.tm_hour %= 24;
    }

    cmos_write(CMOS_HOUR, bin_to_bcd(time.tm_hour));
    cmos_write(CMOS_MINUTE, bin_to_bcd(time.tm_min));
    cmos_write(CMOS_SECOND, bin_to_bcd(time.tm_sec));

    cmos_write(CMOS_B, 0b00100010); // 打开闹钟中断
    cmos_read(CMOS_C);              // 读 C 寄存器，以允许 CMOS 中断
}
static void rtc_handler(u32 vector)
{
    BMB;
    // 实时时钟中断向量号
    assert(vector == 0x28);

    send_eoi(vector);// 向中断控制器发送中断处理完成信号

    cmos_read(CMOS_C);

    // set_alarm(1);

    LOGK("rtc handler %d...\n", counter++);
}

void rtc_init()
{
    LOGK("rtc init\n");

    cmos_write(CMOS_B, 0b01000010);// 打开周期中断
    // cmos_write(CMOS_B, 0b00100010);// 打开闹钟中断
    cmos_read(CMOS_C);// 读coms_c， 运行coms中断

    // set_alarm(5);

    // 设置中断频率
    cmos_write(CMOS_A, (inb(CMOS_A) & 0xf) | 0b1110);
    // hang();
    set_interrupt_handler(IRQ_RTC, rtc_handler);
    set_interrupt_mask(IRQ_RTC, true);
    set_interrupt_mask(IRQ_CASCADE, true);
}


