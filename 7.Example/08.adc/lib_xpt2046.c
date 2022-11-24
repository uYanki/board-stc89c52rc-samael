#include "lib_xpt2046.h"

spi_t spi_xpt2046;

// #define SPI_MOSI P3_4
// #define SPI_MISO P3_7
// #define SPI_SCLK P3_6
// #define SPI_CS P3_5

void XPT2046_Init() {
    spi_xpt2046.firstbit = SPI_FIRSTBIT_MSB;
    spi_xpt2046.datasize = SPI_DATASIZE_8;
    spi_xpt2046.cpha     = SPI_CPHA_2EDGE;
    spi_xpt2046.cpol     = SPI_CPOL_HIGH;
}

extern void delay_ms(uint16_t ms);

uint16_t XPT2046_Read(uint8_t cmd) {
    uint16_t ad = 0;
    SPI_CS      = 0;
    spi_rw(&spi_xpt2046, cmd);
    delay_ms(1);  // 等待转换
    ad = spi_rw(&spi_xpt2046, 0) << 8;
    ad |= (spi_rw(&spi_xpt2046, 0));
    ad >>= 3;
    SPI_CS = 1;
    return ad;
}
