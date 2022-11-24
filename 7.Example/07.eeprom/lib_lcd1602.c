#include "lib_lcd1602.h"

/* LCD1602

1602:
- 可显示2行, 每行能显示16个字符

引脚:
- VL: 对比度调整, 接近VDD时对比度最弱, 接近VSS时对比度最强.
- RS: 寄存器选择, 高电平选择数据寄存器, 低电平选择指令寄存器
- RW: 读写信号线, 高电平进行读操作, 低电平进行写操作
    - RS=RW=0   可写入指令或显示地址
    - RS=1,RW=1 可读取忙信号
    - RS=1,RW=0 可写入数据
- EN: 使能端, 其在高电平变低电平(下降沿)时, 液晶模块执行命令
- D0-D7: 8位双向数据线
- BLA: 背光正极
- BLK: 背光负极

*/

/* 命令集
- 显示模式设置
    [0b00111000]
- 清屏幕
    [0b00000001]
- 光标返回
    [0b0000001x]
- 置输入模式
    [0b000001NS]
    N=1: 当读或写1个字符后,地址指针和光标都加1
    N=0: 当读或写1个字符后,地址指针和光标都减1
    S=1: 当写1个字符后,屏幕显示左移动(N=1)或右移(N=0),以得到光标不移动而屏幕移动的效果
    S=0: 当写1个字符后,整屏显示不移动
- 显示开关控制
    [0b00001DCB]
    D=1:开显示   D=0:关显示   (控制液晶屏的亮灭)
    C=1:显示光标 C=0:不显示光标 (控制光标的显示)
    B=1:光标闪烁 B=0:光标不闪烁
- 数据存贮器地址
    [0b01xxxxxx]
    0x80 | 0x00 -> 0x80 第一行第一个位置
    0x80 | 0x40 -> 0xC0 第二行第一个位置
    显示区域寄存器:
    第一行: 0x00 ~ 0x10
    第二行: 0x40 ~ 0x50
*/

// delay,1ms @11.0592MHz
void LCD1602_Delay() {
    uint8_t i, j;
    i = 2;
    j = 199;
    do {
        while (--j)
            ;
    } while (--i);
}

void LCD1602_WriteCmd(uint8_t byte) {  // 写指令
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    // LCD1602_EN = 0;
    LCD1602_DB = byte;
    LCD1602_EN = 1;
    LCD1602_Delay();
    LCD1602_EN = 0;
    LCD1602_Delay();
}

void LCD1602_WriteData(uint8_t byte) {  // 写数据
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    // LCD1602_EN = 0;
    LCD1602_DB = byte;
    LCD1602_EN = 1;
    LCD1602_Delay();
    LCD1602_EN = 0;
    LCD1602_Delay();
}

void LCD1602_Init() {
    LCD1602_WriteCmd(0x38);  // 0b00111000 显示模式设置
    LCD1602_WriteCmd(0x0C);  // 0b00001100 开启显示开关但不闪烁
    LCD1602_WriteCmd(0x06);  // 0b00000110 显示光标移动位置
    LCD1602_WriteCmd(0x01);  // 0b00000001 清除屏幕
}

void LCD1602_On() { LCD1602_WriteCmd(0x0C); }  /*开启显示屏*/
void LCD1602_Off() { LCD1602_WriteCmd(0x08); } /*关闭显示屏*/

void LCD1602_SetCursor(uint8_t row /*0~1*/, uint8_t col /*0~15*/) {  //
    LCD1602_WriteCmd(0x80 + (row ? 0x40 : 0x00) + col);
}

// 清屏
void LCD1602_Clear() { LCD1602_WriteCmd(0x01); }

// 显示字符
void LCD1602_ShowChar(uint8_t ch) { LCD1602_WriteData(ch); }
void LCD1602_ShowStr(uint8_t* str) {
    while (*str) LCD1602_WriteData(*str++);
}
void LCD1602_ShowStrEx(uint8_t row, uint8_t col, uint8_t* str) {
    uint8_t i = 16 - col;
    if (row > 1 || col > 15) return;
    LCD1602_SetCursor(row, col);
    while (*str) {
        LCD1602_WriteData(*str++);
        if (--i == 0) break;
    }
    if (row) return;  //自动换行显示
    LCD1602_SetCursor(1, 0);
    i = 16;
    while (*str) {
        LCD1602_WriteData(*str++);
        if (--i == 0) break;
    }
}

// 显示数字
long POW(int x, int y) {
    long result = 1;
    while (y--) result *= x;
    return result;
}
void LCD1602_ShowBinNum(unsigned long num, uint8_t len) {
    while (len--) LCD1602_WriteData('0' + (num >> len & 1));
}
void LCD1602_ShowDecNum(unsigned long num, uint8_t len) {
    while (len--) LCD1602_WriteData('0' + num / POW(10, len) % 10);
}
void LCD1602_ShowSignedNum(signed long num, uint8_t len) {
    if (num < 0) {
        LCD1602_WriteData('-');
        LCD1602_ShowDecNum(-num, len);
        return;
    }
    LCD1602_WriteData('+');
    LCD1602_ShowDecNum(num, len);
}
void LCD1602_ShowHexNum(unsigned long num, uint8_t len) {
    unsigned long single;
    while (len--) {
        single = num / POW(16, len) % 16;
        LCD1602_WriteData((single < 10 ? '0' : ('A' - 10)) + single);
    }
}
