#include "isp.h"


sfr IAP_CONTR = 0xE7;

void isp_init(void) // baudrate: 9600
{
    TMOD = 0x20;
    SM0  = 0;
    SM1  = 1;
    REN  = 1;
    TH1  = 0xFd;
    TL1  = 0xFd;
    TR1  = 1;
    EA   = 1;
    ES   = 1;
}

void isr_uart(void) interrupt 4
{
    unsigned char k = 0;
    unsigned int  a, b;
    if (RI == 1) {
        RI = 0;
        k  = SBUF;
        if (k == 0x3D) {  // 0x3D: ISP Download Command
            for (a = 1000; a > 0; a--)
                for (b = 100; b > 0; b--)
                    ;
            IAP_CONTR = 0x60;
        }
    } else {
        TI = 0;
    }
}