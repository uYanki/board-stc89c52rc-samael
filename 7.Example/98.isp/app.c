#include "defs.h"
#include "board.h"
#include "isp/isp.h"

#define LED_MODE 1

void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void main()
{
    isp_init();
    while (1) {
			
#if LED_MODE
        LED_R = 0;
#else
        LED_G = 0;
#endif
			
        delay_ms(100);
			
#if LED_MODE
        LED_R = 1;
#else
        LED_G = 1;
#endif
			
        delay_ms(100);
    }
}



