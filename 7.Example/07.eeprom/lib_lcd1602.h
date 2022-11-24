#ifndef LIB_LCD1602
#define LIB_LCD1602

#include <REGX52.H>

#define LCD1602_RS P2_6  // 1数据 0指令
#define LCD1602_RW P2_5  // 1写入 0读出
#define LCD1602_EN P2_7
#define LCD1602_DB P0

#ifndef _LIB_FUNC_
#define _LIB_FUNC_

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#endif

void LCD1602_Init();

void LCD1602_On();  /*开启显示屏*/
void LCD1602_Off(); /*关闭显示屏*/

void LCD1602_SetCursor(uint8_t row /*0~1*/, uint8_t col /*0~15*/);

// 清屏
void LCD1602_Clear();

// 显示字符
void LCD1602_ShowChar(uint8_t ch);
void LCD1602_ShowStr(uint8_t* str);
void LCD1602_ShowStrEx(uint8_t row, uint8_t col, uint8_t* str);  // 带自动换行

// 显示数字
void LCD1602_ShowBinNum(unsigned long num, uint8_t len);
void LCD1602_ShowDecNum(unsigned long num, uint8_t len);
void LCD1602_ShowSignedNum(signed long num, uint8_t len);
void LCD1602_ShowHexNum(unsigned long num, uint8_t len);

#endif
