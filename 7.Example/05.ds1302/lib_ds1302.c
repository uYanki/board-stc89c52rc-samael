
#include "lib_ds1302.h"

/*

RTC: real time clock 实时时钟

CE/RST: chip enable 片选脚, 高电平选中
SCLK: serial clock 串行时钟线
I/O: 数据线

注: DS1302不是标准的SPI,尽量不用SPI协议

*/

void ds1302_init() {
    DS1302_RST  = 0;
    DS1302_SCLK = 0;
    DS1302_IO   = 0;
}

void ds1302_write(uint8_t reg, uint8_t byte) {
    uint8_t i   = 0;
    DS1302_RST  = 0;
    DS1302_SCLK = 0;
    DS1302_RST  = 1;
    for (i = 0; i < 8; ++i) {
        DS1302_IO   = reg & (1 << i);
        DS1302_SCLK = 0;
        DS1302_SCLK = 1;
    }
    for (i = 0; i < 8; ++i) {
        DS1302_IO   = byte & (1 << i);
        DS1302_SCLK = 0;
        DS1302_SCLK = 1;
    }
    DS1302_RST = 0;
}

uint8_t ds1302_read(reg) {
    uint8_t i   = 0;
    uint8_t buf = 0;
    reg |= 0x01;
    DS1302_RST = 1;
    // 上升沿前写入数据
    for (i = 0; i < 8; ++i) {
        DS1302_IO   = reg & (1 << i);
        DS1302_SCLK = 0;
        DS1302_SCLK = 1;
    }
    // 下降沿后读出数据
    for (i = 0; i < 8; ++i) {
        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
        if (DS1302_IO) buf |= (1 << i);
    }
    DS1302_RST = 0;
    DS1302_IO  = 0;
    return buf;
}

uint8_t BCD2DEC(uint8_t bcd) { return bcd / 16 * 10 + bcd % 16; }
uint8_t DEC2BCD(uint8_t dec) { return dec / 10 * 16 + dec % 10; }

// eg: uint8_t time[6] = {11, 11, 11, 16, 26, 36}; /*年月日时分秒*/

/* 设置时间 需传入长度为6的数组 从低到高分别代表年月日时分秒 */
void ds1302_settime(unsigned char time[] /* 年月日时分秒 len(arr)=6*/) {
    ds1302_write(DS1302_WP, 0x00);  //关闭写保护
    ds1302_write(DS1302_YEAR, DEC2BCD(time[0]));
    ds1302_write(DS1302_MONTH, DEC2BCD(time[1]));
    ds1302_write(DS1302_DATE, DEC2BCD(time[2]));
    ds1302_write(DS1302_HOUR, DEC2BCD(time[3]));
    ds1302_write(DS1302_MINUTE, DEC2BCD(time[4]));
    ds1302_write(DS1302_SECOND, DEC2BCD(time[5]));
    ds1302_write(DS1302_WP, 0xff);  //开启写保护
}

/* 读取时间 需传入长度为6的数组 从低到高分别代表年月日时分秒 */
void ds1302_get_time(unsigned char time[] /* 年月日时分秒 len(arr)=6*/) {
    time[0] = BCD2DEC(ds1302_read(DS1302_YEAR));
    time[1] = BCD2DEC(ds1302_read(DS1302_MONTH));
    time[2] = BCD2DEC(ds1302_read(DS1302_DATE));
    time[3] = BCD2DEC(ds1302_read(DS1302_HOUR));
    time[4] = BCD2DEC(ds1302_read(DS1302_MINUTE));
    time[5] = BCD2DEC(ds1302_read(DS1302_SECOND));
}
