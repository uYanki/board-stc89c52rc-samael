
#ifndef LIB_AT24C02
#define LIB_AT24C02

#include "lib_i2c.h"

#define AT24C02_ADDRESS 0x50  // 自行扫描地址

uint8_t AT24C02_WriteByte(uint8_t reg_addr, uint8_t byte);
uint8_t AT24C02_WriteBytes(uint8_t reg_addr, uint8_t* buf, uint8_t len);
uint8_t AT24C02_ReadByte(uint8_t reg_addr, uint8_t* byte);
uint8_t AT24C02_ReadBytes(uint8_t reg_addr, uint8_t* buf, uint8_t len);

#endif
