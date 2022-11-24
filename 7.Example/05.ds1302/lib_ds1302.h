#ifndef LIB_DS1302
#define LIB_DS1302

#include <REGX52.H>

#ifndef _uint_
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#define _uint_
#endif

#define DS1302_RST P3_5
#define DS1302_SCLK P3_6
#define DS1302_IO P3_4

// 最低位为1代表读,为0代表写
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_DATE 0x86
#define DS1302_MONTH 0x88
#define DS1302_DAY 0x8A
#define DS1302_YEAR 0x8C
#define DS1302_WP 0x8E  // 写保护位 0不保护 1保护

#include "lib_ds1302.h"

/*

RTC: real time clock 实时时钟

CE/RST: chip enable 片选脚, 高电平选中
SCLK: serial clock 串行时钟线
I/O: 数据线

注: DS1302不是标准的SPI,尽量不用SPI协议

*/

void    ds1302_init();
void    ds1302_write(uint8_t reg, uint8_t byte);
uint8_t ds1302_read(cmd);
void    ds1302_settime(unsigned char time[] /* 年月日时分秒 len(arr)=6*/);
void    ds1302_get_time(unsigned char time[] /* 年月日时分秒 len(arr)=6*/);

#endif
