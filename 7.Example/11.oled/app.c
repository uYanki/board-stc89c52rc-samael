#include <REGX52.H>

#include "lib_oled.h"
// extern void delay_ms(uint16_t ms);

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void main() {
    // OLED_StopScroll();
    delay_ms(200);
    OLED_Init();
    // OLED_Fill(0xF0);

    OLED_ShowStrL(4, 44,
                  "OLED_WriteData(0xFF);"
                  "OLED_Show"
                  "OLED_ShowChar('8');"
                  "OLED_Show"
                  "OLED_Show"
                  "OLED_ShowC");

    // OLED_HorizontalScroll(1, 0x00, 0x06);

// 垂直滚动 (试验没成功)
#define start 0x00
#define stop 0x0F
    OLED_StopScroll();
    // 设置上下滚动区域
    OLED_WriteCmd(0xA3);
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(SSD1306_LCDHEIGHT);
    OLED_WriteCmd(0X29);  // right
    OLED_WriteCmd(0X00);
    // 设置左右滚动页
    OLED_WriteCmd(start);
    OLED_WriteCmd(0X00);
    OLED_WriteCmd(stop);
    OLED_WriteCmd(0x01);
    OLED_StartScroll();

    while (1) {
    }
}
