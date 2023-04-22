#ifndef __ONIX_RTC_H__
#define __ONIX_RTC_H__

#include <onix/types.h>


u8 cmos_read(u8 addr);
void cmos_write(u8 addr, u8 value);
void rtc_init();

#endif