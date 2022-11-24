#ifndef LIB_OLED
#define LIB_OLED

#include "lib_i2c.h"

// fpga demo: https://blog.csdn.net/lum250/article/details/121439597
// stm oled: https://www.cnblogs.com/yilangUAV/p/12727606.html
// oled 滚动: https://blog.csdn.net/junseven164/article/details/121450137

/* 128x64 OLED ( Organic Light Emiting Diode )

屏幕显存 GDDRAM :
- 有8页->每页大小=128bit(w)*8bit(h)
- 每页有8行比特 -> 每页8行COM
- 每页有128列比特 -> 每列128列SEG
- 没比特代表1个像素点

    [0] 0 1 2 3 ... 127
    [1] 0 1 2 3 ... 127
    [2] 0 1 2 3 ... 127
    [3] 0 1 2 3 ... 127
    [4] 0 1 2 3 ... 127
    [5] 0 1 2 3 ... 127
    [6] 0 1 2 3 ... 127
    [7] 0 1 2 3 ... 127

先通过 OLED_WriteCmd(0xB0|i); 选中指定页 (0<=i<=7)
再使用 OLED_WriteCmd(0x00)(低4为); OLED_WriteCmd(0x10)(高4位); 设置列索引
最后使用 OLED_WriteData(byte); 设置该列8个比特位的内容


页地址模式: 每次对GDDRAM进行读写之后,列地址都会自增1. 在达到列终止地址后, 会回到列起始地址, 但页地址不会发生变化.
水平地址模式: 每次对GDDRAM进行读写之后,列地址都会自增1. 在达到列终止地址后, 会回到列起始地址, 且页地址会自增1.
-- 在列地址和页地址都到达各自的终止地址后, 会回到各自的起始地址.
垂直地址模式: 每次对GDDRAM进行读写之后,页地址都会自增1. 在达到页终止地址后, 会回到页起始地址, 且列地址会自增1.
-- 在列地址和页地址都到达各自的终止地址后, 会回到各自的起始地址.

*/

#define SSD1306_ADDR 0X3C

#define FONT_6x8 1
#define SSD1306_128_64
#define SSD1306_ENABLE_SCROLL 1

// screen size (代码里没用上)
#if defined SSD1306_128_64
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64
#endif
#if defined SSD1306_128_32
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 32
#endif
#if defined SSD1306_96_16
#define SSD1306_LCDWIDTH 96
#define SSD1306_LCDHEIGHT 16
#endif

#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29  // right and veritcal
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A   // left and veritcal
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3              // set scroll range

// Co D/C 0000000 -> Co=0,D/C=0:0x00 (command 命令), Co=0,D/C=1:0x40 (data 数据)
#define OLED_WriteCmd(cmd) i2c_write_byte(SSD1306_ADDR, 0x00, cmd)
#define OLED_WriteData(byte) i2c_write_byte(SSD1306_ADDR, 0x40, byte)

void OLED_Init(void);
void OLED_On(void);
void OLED_Off(void);
void OLED_Fill(uint8_t byte);
#define OLED_Clear() OLED_Fill(0x00)

void OLED_Init(void);
void OLED_Fill(uint8_t byte);

void OLED_On(void);
void OLED_Off(void);

void OLED_SetCursor(uint8_t page /* 0~7 */, uint8_t col /* 0~127 */);
void OLED_SetContrast(uint8_t contrast);                    // 对比度
void OLED_SetInvert(uint8_t bool /* normal:0,invert:1 */);  // 屏幕反相

#if SSD1306_ENABLE_SCROLL
#define OLED_StartScroll() OLED_WriteCmd(0x2F)
#define OLED_StopScroll() OLED_WriteCmd(0x2E)
// 水平滚动, To scroll the whole display, run: display.startscrollleft(0x00, 0x07)
void OLED_HorizontalScroll(uint8_t dir /*0=left,1=right*/, uint8_t begin /* page: 0~7 */, uint8_t end /* page: 0~7 */);
#endif

// 我看了Arduino 的 OLED 库, 基本都是创建大数组做缓冲区,
// 然后整个缓冲区写入的 OLED, 我这个代码是直接往 OLED 里写入的
// 而使用缓冲区的好处, 就是方便设置某点的 黑、白、反相

void OLED_ShowChar(uint8_t ch);
void OLED_ShowChar8x16(uint8_t ch);
void OLED_ShowStr(uint8_t* str);
void OLED_ShowNum(uint16_t num) reentrant;
void OLED_ShowHex(uint16_t num) reentrant;

// 显示长文本, 会自动换行
void OLED_ShowStrL(uint8_t page, uint8_t col, uint8_t* str);

#endif
