

#include "lib_uart.h"

void uart_init(void) {  // @baud=9600
    SCON = 0x50;        // 模式1:8位UART,波特率可变,启用接收 . SM0=0;SM1=1;REN=1;
    PCON |= 0x80;       // 设置波特率加倍, 波特率倍速位 SMOD=1, SMOD0=0
    ES = 1;             // 串口开关
    TMOD &= 0x0f;       // timer mode,清除原定时器工作模式
    TMOD |= 0x20;       // 设置为八位自动重载
    TH1 = TL1 = 0xFA;   // 赋定时器初值
    ET1       = 0;      // disenable timer1, 关闭定时器1的中断
    TR1       = 1;      // timer1 run, 启动定时器1
}

void uart_sendchar(char byte) {
    SBUF = byte;    //传入数据
    while (!TI) {}  //等待发送完毕
    TI = 0;         //将数据发送标志位复位
}

void uart_sendline(void) {  // 换行
    uart_sendchar('\r');
    uart_sendchar('\n');
}

void uart_sendstr(char* string) {
    while (*string) uart_sendchar(*string++);
}

void uart_sendstr_ln(char* string) {
    uart_sendstr(string);
    uart_sendline();
}

void uart_senddec(int number) reentrant {
    if (number > 9)
        uart_senddec(number / 10);
    uart_sendchar('0' + number % 10);
}

void uart_sendhex(int number) reentrant {
    char val;
    if (number > 15)
        uart_sendhex(number / 16);
    val = number % 16;
    if (val < 10)
        uart_sendchar('0' + val);
    else
        uart_sendchar('A' + val - 10);
}

void uart_isr() interrupt 4 {
    if (RI)  //数据接收标志位
    {
        uart_handle(SBUF);  // SBUF 为接受到的数据
        RI = 0;             //将数据接收标志位复位
    }
}
