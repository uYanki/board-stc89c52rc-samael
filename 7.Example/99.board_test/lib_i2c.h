#ifndef LIB_I2C
#define LIB_I2C

#include "app.h"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

uint8_t i2c_check(uint8_t addr_dev);  // 检查器件是否存在
uint8_t i2c_write_byte(uint8_t addr_dev, uint8_t addr_reg, uint8_t byte);
uint8_t i2c_read_byte(uint8_t addr_dev, uint8_t addr_reg, uint8_t* byte);
uint8_t i2c_write_bytes(uint8_t addr_dev, uint8_t addr_reg, uint8_t* buf, uint8_t len);
uint8_t i2c_read_bytes(uint8_t addr_dev, uint8_t addr_reg, uint8_t* buf, uint8_t len);

#endif

/* 扫描器件地址

#include "lib_i2c.h"
#include "lib_lcd1602.h"

void i2c_scan() {
    uint8_t i;
    for (i = 0; i < 0x80; i++) {
        if (i2c_check(i)) {
            LCD1602_SetCursor(0, 0);
            LCD1602_ShowHexNum(i, 2);
            delay_ms(3000);
        }
        delay_ms(1);
    }
}

void main() {
    LCD1602_Init();
    while (1) { i2c_scan(); }
}

*/
