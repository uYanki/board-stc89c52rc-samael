
#include "lib_i2c.h"
#include "lib_uart.h"

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void i2c_scan() {
    uint8_t i;
     uint8_t b = 0;
    for (i = 0; i < 0x80; i++) {
        if (i2c_check(i)) {
            b = 1;
            uart_sendstr("0x");
            uart_sendhex(i);
            uart_sendchar(' ');
            delay_ms(3000);
        }
        delay_ms(1);
    }

    if (b)
        uart_sendline();
    else
        uart_sendstr_ln("none");
}

void main() {
    uart_init();
    while (1) {
        delay_ms(5000);
        i2c_scan();
    }
}
