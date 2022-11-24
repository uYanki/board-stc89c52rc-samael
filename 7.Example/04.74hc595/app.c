
/* LED Matrix */

#include "lib_74hc595.h"

void main() {
    P0 = 0x00;
    _74HC595_WriteByte(6);  // 0b00000110
    while (1) {}
}
