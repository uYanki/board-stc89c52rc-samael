#include <INTRINS.H>  // import _nop_()
#include <REGX52.H>

#ifndef __APP_H__
#define __APP_H__

// P4
sfr  P4   = 0xE8;
sbit P4_0 = 0xE8;
sbit P4_1 = 0xE9;
sbit P4_2 = 0xEA;
sbit P4_3 = 0xEB;
sbit P4_4 = 0xEC;
sbit P4_5 = 0xED;
sbit P4_6 = 0xEE;
sbit P4_7 = 0xEF;

sfr XICON = 0xC0;
// INT2
sbit IT2 = 0xC0;
sbit IE2 = 0xC1;
sbit PX2 = 0xC2;
sbit EX2 = 0xC3;
// INT3
sbit IT3 = 0xC4;
sbit IE3 = 0xC5;
sbit EX3 = 0xC6;
sbit PX3 = 0xC7;

// PIN_INT
#define INT0 P3_2
#define INT1 P3_3
#define INT2 P4_3
#define INT3 P4_2

// typedef
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

// delay @11.0592MHz
void delay_ms(uint8_t ms);
void delay_10us(uint16_t ten_us);

// led (低电平亮, 高电平灭)
#define LED P4_1
#define LED_R P2_5
#define LED_G P2_6
#define LED_B P2_7

// key
#define SW1 INT1
#define SW2 INT0
#define SW1_ISR 2
#define SW2_ISR 0

// eeprom (at24c02)
#define I2C_SDA P0_6
#define I2C_SCL P0_7

// ired
#define IRED INT3
#define IRED_ISR 3

// timer
#define TIMER0_ISR 1
#define TIMER1_ISR 3

/* 遥控器码表
0x45 0x46 0x47
0x44 0x40 0x43
0x07 0x15 0x09
0x16 0x19 0x0D
0x0C 0x18 0x5E  (1 2 3)
0x08 0x1C 0x5A  (4 5 6)
0x42 0x52 0x4a  (7 8 9)
*/

// uart
#define BAUD 9600  // 波特率

// handle
void uart_handle(uint8_t recv);
void ired_handle(uint8_t recv);
void key1_handle(void);
void key2_handle(void);
char timer0_handle(uint32_t cnt);  // cnt 为定时器计数, 返回 1 清除计数
// char timer1_handle(uint32_t cnt);

// 注意, timer 其实是用了 P4 口的中断, 因为 timer1 的中断脚给 ired 用了, 除非把红外接收头拆下来, 否则用不了 timer1
// 又因为 timer0 的中断优先级 比 ired 高 (可能是这样), 导致在开启 timer0 时用不了 ired

#endif
