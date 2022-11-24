#ifndef LIB_SEG
#define LIB_SEG

#include <REGX52.H>

#ifndef _uint_
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#define _uint_
#endif

extern void delay_ms(uint16_t ms);

void seg_set(long num, uint8_t base /*2~16进制*/);

#endif
