#include <REGX52.H>
#include "lib_seg.h"
#include "lib_xpt2046.h"

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void main() {
    uint32_t val;
    XPT2046_Init();
    while (1) {
        val = XPT2046_Read(XPT2046_AD_CH0);
        seg_set(val, 10);
    }
}
