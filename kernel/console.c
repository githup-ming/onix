#include <onix/console.h>
#include <onix/io.h>
#include <onix/string.h>

#define CRT_ADDR_REG 0x3D4 // CRT(6845)索引寄存器
#define CRT_DATA_REG 0x3D5 // CRT(6845)数据寄存器

#define CRT_START_ADDR_H 0xC // 显示内存起始位置 - 高位
#define CRT_START_ADDR_L 0xD // 显示内存起始位置 - 低位
#define CRT_CURSOR_H 0xE     // 光标位置 - 高位
#define CRT_CURSOR_L 0xF     // 光标位置 - 低位

#define MEM_BASE 0xB8000              // 显卡内存起始位置
#define MEM_SIZE 0x4000 - 64               // 显卡内存大小，4整屏余2行，余下64字节不要了不够1行
#define MEM_END (MEM_BASE + MEM_SIZE) // 显卡内存结束位置
#define WIDTH 80                      // 屏幕文本列数
#define HEIGHT 25                     // 屏幕文本行数
#define ROW_SIZE (WIDTH * 2)          // 每行字节数
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // 屏幕字节数
#define attr 7                      // 字符样式
#define erase 0x0720                     // 空格


#define NUL 0x00
#define ENQ 0x05
#define ESC 0x1B // ESC
#define BEL 0x07 // \a
#define BS 0x08  // \b
#define HT 0x09  // \t
#define LF 0x0A  // \n
#define VT 0x0B  // \v
#define FF 0x0C  // \f
#define CR 0x0D  // \r
#define DEL 0x7F

struct console
{
    u32 screen; //当前显示器开始内存位置
    u32 pos; //记录当前光标的内存位置
    struct point xy; //当前光标的坐标
};

struct console console;

//获取屏幕开始的内存位置
static u32 get_screen()
{
    u32 screen = 0;
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);
    screen = inb(CRT_DATA_REG) << 8;
    outb(CRT_ADDR_REG,CRT_START_ADDR_L);
    screen |= inb(CRT_DATA_REG);

    screen = screen * 2 + MEM_BASE;

    return screen;
}
//获取当前光标的内存位置
static u32 get_cursor()
{
    u32 pos = 0;
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    pos = inb(CRT_DATA_REG) << 8;
    outb(CRT_ADDR_REG,CRT_CURSOR_L);
    pos |= inb(CRT_DATA_REG);

    pos = pos * 2 + MEM_BASE;//一个字符占2个字节（字符，样式）

    return pos;
}
//光标内存位置转换为屏幕坐标
static struct point pos_to_xy(u32 pos)
{
    struct point point;
    pos = (pos - MEM_BASE) / 2;

    point.x = pos % WIDTH;
    point.y = pos / WIDTH;

    return point;
}

//设置屏幕光标位置
static void set_cursor(struct point xy)
{
    if (xy.x >= WIDTH) {
        xy.x = WIDTH - 1;
    }
    if (xy.y >= MEM_SIZE / ROW_SIZE ) {//<=101
        xy.y = MEM_SIZE / ROW_SIZE - 1;
    }
    u16 point = xy.y * 80 + xy.x;

    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    outb(CRT_DATA_REG, (point >> 8) & 0xff);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    outb(CRT_DATA_REG, point & 0xff);
}
//设置屏幕开始坐标位置
static void set_screen(u32 mem)
{
    u16 point = (mem - MEM_BASE) / 2;

    outb(CRT_ADDR_REG, CRT_START_ADDR_H);
    outb(CRT_DATA_REG, point >> 8);
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    outb(CRT_DATA_REG, point & 0xff);

    console.screen = mem;
}

static void command_lf()
{
    if (console.pos < (MEM_END - ROW_SIZE)) {
        console.xy.y++;
        console.pos += ROW_SIZE;
        if ((console.pos - console.screen) > (SCR_SIZE -ROW_SIZE)) {
            set_screen(console.screen + ROW_SIZE);
        }
    } else {
        memcpy((void *)MEM_BASE, (void *)console.screen, SCR_SIZE);
        u16 *ptr = (u16 *)(MEM_BASE + SCR_SIZE);

        while ((u32)ptr < MEM_END){
            *ptr++ = erase;
        }

        set_screen(MEM_BASE + ROW_SIZE);
        console.pos -= (MEM_SIZE - SCR_SIZE - ROW_SIZE);
        console.xy = pos_to_xy(console.pos);
    }
}

static void command_cr()
{
    console.pos -= console.xy.x * 2;
    console.xy.x = 0;
}

static void command_bs()
{
    if (console.xy.x) {
        console.xy.x--;
        console.pos -= 2;
        *(u16 *)console.pos = erase;
    }
}
static void command_del()
{
    *(u16 *)console.pos = erase;

}



void console_init()
{
    console_clear();

    console.screen = get_screen();
    console.pos = get_cursor();
    console.xy = pos_to_xy(console.pos);
}

void console_clear()
{
    u16 *ptr = (u16 *)MEM_BASE;
    struct point xy = pos_to_xy((u32)ptr);

    while ((u32)ptr < MEM_END){
        *ptr++ = erase;
    }

    set_cursor(xy);
}
extern void start_beep();
void console_write(int8 *buf, u32 count)
{
    int8 ch;
    while (count--)
    {
        ch = *buf++;
        switch (ch)
        {
            case NUL:
                break;
            case ENQ:
                break;
            case ESC:
                break;
            case BEL:
                start_beep();
                break;
            case BS:
                command_bs();
                break;
            case HT:
                break;
            case LF:
                command_lf();
                command_cr();
                break;
            case VT:
                break;
            case FF:
                command_lf();
                break;
            case CR:
                command_cr();
                break;
            case DEL:
                command_del();
                break;
            default:
                *(u8 *)console.pos++ = ch;
                *(u8 *)console.pos++ = attr;

                if (console.pos < MEM_END ) {
                    console.xy = pos_to_xy(console.pos);
                    if (console.pos - console.screen >= SCR_SIZE) {
                        set_screen(console.screen + ROW_SIZE);
                    }
                } else {
                    memcpy((void *)MEM_BASE, (void *)console.screen, SCR_SIZE);
                    u16 *ptr = (u16 *)(MEM_BASE + SCR_SIZE);

                    while ((u32)ptr < MEM_END){
                        *ptr++ = erase;
                    }

                    set_screen(MEM_BASE + ROW_SIZE);
                    console.pos -= (MEM_SIZE - SCR_SIZE);
                    console.xy = pos_to_xy(console.pos);
                }
                break;
        }
        set_cursor(console.xy);
    }
}