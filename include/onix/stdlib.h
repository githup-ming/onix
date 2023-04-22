#ifndef __ONIX_STDLIB_H__
#define __ONIX_STDLIB_H__

#include <onix/types.h>

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a > b ? b : a)

void delay(u32 count);
void hang();

u8 bcd_to_bin(u8 value);
u8 bin_to_bcd(u8 value);

u32 div_round_up(u32 num, u32 size);

#endif