#include "lib_at24c02.h"

/* AT24Cxx, II2-EEPROM(ROM) 即电可擦除可编程只读存储器

WP: Write Protect 写保护, 接高电平时只可读不可写, 接低电平时可读又可写

用途: 在掉电时也可保存数据.

AT24C02 内存 2Kbit(256byte=2^8) -> 字地址是8位的

字地址位要你要往哪个寄存器写入数据的地址.

注意!!!! 在写入之后,至少需要延时2ms才能读取,否则会导致读取失效

*/

uint8_t AT24C02_WriteByte(uint8_t reg_addr, uint8_t byte) { return i2c_write_byte(AT24C02_ADDRESS, reg_addr, byte); }
uint8_t AT24C02_ReadByte(uint8_t reg_addr, uint8_t* byte) { return i2c_read_byte(AT24C02_ADDRESS, reg_addr, byte); }
uint8_t AT24C02_WriteBytes(uint8_t reg_addr, uint8_t* buf, uint8_t len) {
    uint8_t i;
    if ((reg_addr + len) > 255) return 0;
    for (i = 0; i < len; ++i)
        if (!i2c_write_byte(AT24C02_ADDRESS, reg_addr++, buf[i])) return 0;
    delay_ms(3);
    return 1;
}
uint8_t AT24C02_ReadBytes(uint8_t reg_addr, uint8_t* buf, uint8_t len) {
    uint8_t i;
    if ((reg_addr + len) > 255) return 0;
    for (i = 0; i < len; ++i)
        if (!i2c_read_byte(AT24C02_ADDRESS, reg_addr++, buf + i)) return 0;
    delay_ms(3);
    return 1;
}
