#include "lib_key.h"

// 外部中断 exti

void key1_init(void) {
    IT1 = 1;
    EX1 = 1;
    SW1 = 1;
}

void key1_isr() interrupt SW1_ISR {
    delay_ms(10);  //消抖
    if (SW1 == 0)  //再次判断按键状态
        key1_handle();
}

void key2_init(void) {
    IT0 = 1;
    EX0 = 1;
    SW2 = 1;
}
void key2_isr() interrupt SW2_ISR {
    delay_ms(10);  //消抖
    if (SW2 == 0)
        key2_handle();
}
