#include <REGX52.H>
#include "lib_28BYJ48.h"

// extern void delay_ms(uint16_t ms);

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void main() {
    stepMotor_stop();
    while (1) {
        stepMotor_run(1, 0, 2);
    }
}
