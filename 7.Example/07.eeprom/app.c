#include "lib_at24c02.h"
#include "lib_lcd1602.h"

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void main() {
    uint8_t i;

    uint8_t arr1[] = {20, 8, 7, 14, 3, 2};
    uint8_t arr2[] = {0, 0, 0, 0, 0, 0};
    uint8_t len    = ARRAY_LEN(arr1);

    LCD1602_Init();

    AT24C02_WriteBytes(255 - len, arr1, len);
    AT24C02_ReadBytes(255 - len, arr2, len);

    while (1) {
        for (i = 0; i < len; i++) {
            LCD1602_SetCursor(0, 0);
            LCD1602_ShowDecNum(arr2[i], 2);
            delay_ms(2000);
        }
    }
}
