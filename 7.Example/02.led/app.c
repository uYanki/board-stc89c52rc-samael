#include <INTRINS.H>  // _nop_()
#include <REGX52.H>

sfr  P4  = 0xe8;
sbit P41 = P4 ^ 1;

// 延时 @11.0592MHz
void delay(unsigned char ms) {
    unsigned char i, j;
    while (ms--) {
        _nop_();
        i = 2;
        j = 199;
        do {
            while (--j)
                ;
        } while (--i);
    }
}

// 流水灯
void main() {
    int i = 0;
    while (1) {
        P2  = ~(1 << i);
        P41 = ~P41;
        delay(200);
        i = (++i) % 8;
    }
}
