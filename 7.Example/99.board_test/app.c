#include "app.h"
#include "lib_at24c02.h"
#include "lib_ired.h"
#include "lib_key.h"
#include "lib_timer.h"
#include "lib_uart.h"

void delay_ms(uint8_t ms) {
    uint8_t i, j;
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
void delay_10us(uint16_t ten_us) {
    while (ten_us--)
        ;
}

void ired_handle(uint8_t recv) {
    // LED = ~LED;

	  uart_sendhex(recv);
		uart_sendline();
	
    switch (recv) {
        // 1 4 7 亮
        case 0x0C: LED_R = 0; break;
        case 0x08: LED_G = 0; break;
        case 0x42: LED_B = 0; break;
        // 3 6 9 灭
        case 0x5E: LED_R = 1; break;
        case 0x5A: LED_G = 1; break;
        case 0x4A: LED_B = 1; break;
        // 5 8
        case 0x1C: LED_R = LED_G = LED_B = 0; break;
        case 0x52: LED_R = LED_G = LED_B = 1; break;
        // other 状态取反
        default: P2 = ~P2; break;
    }
}

#define MEM_ADDR 0x66  // 数据储存地址 ( 0~255 / 0x00~0xFF )
#define RECV_LEN 3     // 接收指定长度的数据

void uart_handle(uint8_t recv_byte) {
    static uint8_t recv_buf[RECV_LEN] = {0};
    static uint8_t recv_len           = 0;  // 当前接收长度

    recv_buf[recv_len++] = recv_byte;
    if (recv_len < RECV_LEN) return;
    recv_len = 0;

    uart_sendstr("rgbled state [count=");
    uart_senddec(RECV_LEN);
    uart_sendstr("]: ");
    uart_sendstr(recv_buf);
    uart_sendstr("\r\n");

    // 串口点灯(低电平亮灯)
    LED_R = !(recv_buf[0] != '0');
    LED_G = !(recv_buf[1] != '0');
    LED_B = !(recv_buf[2] != '0');

    // 格式: 发送3个字符, 对应每个字符控制RGBLED的亮灭, eg: 全亮 111, 全灭 000

    AT24C02_WriteBytes(MEM_ADDR, recv_buf, RECV_LEN);  // 保存LED状态
}

void key1_handle() {
    // 清除LED状态
    uart_sendstr_ln("sw1_clear");
    LED_R = LED_G = LED_B = 1;
}

void key2_handle() {
    // 恢复LED状态
    uint8_t recv_buf[RECV_LEN + 1];
    uart_sendstr_ln("sw2_recover");
    AT24C02_ReadBytes(MEM_ADDR, recv_buf, RECV_LEN);
    recv_buf[RECV_LEN] = '\0';
    uart_sendstr_ln(recv_buf);
    LED_R = !(recv_buf[0] != '0');
    LED_G = !(recv_buf[1] != '0');
    LED_B = !(recv_buf[2] != '0');
}

char timer0_handle(uint32_t cnt) {
    if (cnt > 500) {  // 500ms
        uart_sendstr_ln("timer0");
        LED = ~LED;
        return 1;
    }
    return 0;
}

void main() {
    uart_init();
    key1_init();
    key2_init();
    ired_init();
    // timer0_init();

    EA = 1;  // 总中断开关
    while (1) {
    }
}

// /* I2C 地址扫描

// void i2c_scan() {
//     uint8_t i;
//     for (i = 0; i < 0x80; i++) {
//         if (i2c_check(i)) {
//             uart_sendstr("0x");
//             uart_sendhex(i);
//             uart_sendchar(',');
//             uart_senddec(i);
//             uart_sendline();
//             delay_ms(3000);
//         }
//         delay_ms(1);
//     }
// }

// void main() {
//     uart_init();
//     EA = 1;  // 总中断开关
//     while (1) {
//         i2c_scan();
//     }
// }

// */
