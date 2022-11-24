#include "lib_74hc595.h"

/* 74HC595 串入并出

SER/DS: 串行数据输入口
QH'/Q7': 串行数据输出口(可接另1个74HC595的串行数据输入口,实现级联)
SRCLR/MR: 低电平复位
SRCLK/SHCP: 移位寄存器时钟,上升沿时将串行数据输入口的数据传入移位寄存器
RCLK/STCP: 存储寄存器时钟,上升沿时将移位寄存器的数据传入至存储寄存器

*/

// delay 10us @11.0592MHz
void _74HC595_Delay() {
    unsigned char i;
    i = 26;
    while (--i)
        ;
}

void _74HC595_WriteByte(uint8_t byte) {
    uint8_t i = 8;
    // 上升沿写 移位寄存器
    while (i--) {
        _74HC595_SER;
        _74HC595_SRCLK = 0;
        _74HC595_Delay();
        _74HC595_SRCLK = 1;
        _74HC595_Delay();
    }
    // 上升沿写 储存寄存器
    _74HC595_RCLK = 0;
    _74HC595_Delay();
    _74HC595_RCLK = 1;
}
