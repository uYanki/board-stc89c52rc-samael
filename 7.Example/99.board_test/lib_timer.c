#include "lib_timer.h"

/*

定时器原理:
通过往有容量的水桶里加一定的水, (即设置TH0,TL0的值, 可调节最初的水位)
然后单片机内部每隔一定时间就往桶里加水, 当水满溢出时, 就触发中断函数,
然后需重新设置最初水位, 等待下次的水满时再次触发中断函数...反反复复

*/

// 1ms @11.0592MHz
#define TH0_VALUE 0xFC
#define TL0_VALUE 0x66

// 25ms @11.0592MHz
// #define TH0_VALUE 0xA6
// #define TL0_VALUE 0x00

// 50ms @11.0592MHz
// #define TH0_VALUE 0x4C
// #define TL0_VALUE 0x00

// 70ms @11.0592MHz
// #define TH0_VALUE 0x04
// #define TL0_VALUE 0x00

void timer0_init(void) {
    TMOD |= 0X01;     // timer mode: 1
    TH0 = TH0_VALUE;  // 定时器初值
    TL0 = TL0_VALUE;
    ET0 = 1;  // enable timer
    TR0 = 1;  // timer run
}

void timer0_isr() interrupt 1 {
    static uint32_t i = 0;
    // 重置定时器初值
    TH0 = TH0_VALUE;
    TL0 = TL0_VALUE;
    if (timer0_handle(++i))
        i = 0;
}

// void timer1_init(void) {
//     TMOD |= 0X10;  // timer mode: 1
//     TH1 = 0XFC;    // 定时器初值 1ms
//     TL1 = 0x66;
//     ET1 = 1;  // enable timer
//     TR1 = 1;  // timer run
// }

// void timer1_isr() interrupt 3 using 1 {
//     static uint32_t ik = 0;
//     // 重置定时器初值 1ms
//     TH1 = 0XFC;
//     TL1 = 0x66;
//     if (timer1_handle(++ik))
//         ik = 0;
// }
