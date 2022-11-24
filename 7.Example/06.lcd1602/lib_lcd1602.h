#ifndef LIB_LCD1602
#define LIB_LCD1602

#include <REGX52.H>

#define LCD1602_RS P2_6  // 1数据 0指令
#define LCD1602_RW P2_5  // 1写入 0读出
#define LCD1602_EN P2_7
#define LCD1602_DB P0
#define LCD1602_BUSY P0_7

#ifndef _LIB_FUNC_
#define _LIB_FUNC_

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#endif

//工作方式
#define LCD1602_DISPLAY_DOUBLE_LINE 0x38  // 两行显示
#define LCD1602_DISPLAY_SINGLE_LINE 0x30  // 单行显示

//开关设置
#define LCD1602_DISPLAY_ON 0x0C        // 显示开
#define LCD1602_DISPLAY_OFF 0x08       // 显示关
#define LCD1602_CURSOR_ON 0x0A         // 光标显示
#define LCD1602_CURSOR_OFF 0x08        // 光标不显示
#define LCD1602_CURSOR_BLINK_ON 0x09   // 光标闪烁
#define LCD1602_CURSOR_BLINK_OFF 0x08  // 光标不闪烁

//移动设置
#define LCD1602_LEFT_MOVE 0x18          // 显示左移1位
#define LCD1602_RIGHT_MOVE 0x1C         // 显示右移1位
#define LCD1602_CURSOR_LEFT_MOVE 0x10   // 光标左移1位
#define LCD1602_CURSOR_RIGHT_MOVE 0x14  // 光标右移1位

#define LCD1602_GO_HOME 0x02  // 光标归位

void LCD1602_Init();

void LCD1602_On();  /*开启显示屏*/
void LCD1602_Off(); /*关闭显示屏*/

void LCD1602_SetCursor(uint8_t row /*0~1*/, uint8_t col /*0~15*/);

void LCD1602_WriteCmd(uint8_t byte);
void LCD1602_WriteData(uint8_t byte);

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

void LCD1602_WriteCGRAM(uint8_t pos /* pos of cgram:0~7 */, uint8_t* buf /* 一维数组 */);
void LCD1602_WriteCGRAMs(uint8_t (*buf)[8] /* 二维数组 */, uint8_t len);

#endif
