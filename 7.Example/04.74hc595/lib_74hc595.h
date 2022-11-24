#ifndef LIB_74HC595
#define LIB_74HC595

#include <REGX52.H>

#ifndef _uint_
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#define _uint_
#endif

#define _74HC595_SER P3_4
#define _74HC595_RCLK P3_5
#define _74HC595_SRCLK P3_6

void _74HC595_WriteByte(uint8_t byte);

#endif
