#ifndef LIB_XPT2046
#define LIB_XPT2046

#include <INTRINS.H>
#include "lib_spi.h"

#define SPI_CS P3_5

#define XPT2046_AD_CH0 0x94  // 电位器
#define XPT2046_AD_CH1 0xd4  // 热敏电阻
#define XPT2046_AD_CH2 0xa4  // 光敏电阻
#define XPT2046_AD_CH3 0xe4

/*
XPT2046: 5v 12bitADC(2^12-1=4095), CPHA=1,CPOL=0
如何判断SPI的CPHA&CPOL值? 读出来的AD的最大值能到达4095,就是正确的CPHA&CPOL.
*/

void     XPT2046_Init();
uint32_t XPT2046_Read(uint8_t cmd);

#endif
