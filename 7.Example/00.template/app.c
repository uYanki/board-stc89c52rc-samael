#include <REGX52.H>

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void main() {
    while (1) {}
}
