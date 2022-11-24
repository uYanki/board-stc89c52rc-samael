#include "lib_spi.h"

/* 四线SPI

Refer: https://www.cnblogs.com/muzhishan/articles/10267386.html

CPHA(时钟相位, Clock Polarity): 配置数据采样边沿(0:第1个边沿,1:第2个边沿) -> 数据发送(0:第2个边缘,1:第1个边缘)
    注: 这里的边沿指的是单个周期内,时钟线的跳变沿
CPOL(时钟极性, Clock Phase): 配置时钟线空闲状态的电平(0:低电平,1:高电平)

SPI: Serial Peripheral Interface 串行外设接口
MOSI/DIN: master output, salve input 主设备数据输出,从设备数据输入
MISO/DOUT: master input, salve output  主设备数据输入,从设备数据输出
SCLK: serial clock 串行时钟信号,由主设备产生
CS/SS: chip select / salve select 从设备使能信号,由主设备控制

*/

void spi_w(spi_t* spi, uint16_t buf) {
    uint8_t  i;
    uint16_t startbit = spi->firstbit ? 0x01 : (1 << spi->datasize);
    for (i = 0; i < spi->datasize; i++) {
        if (!spi->cpha) SPI_MOSI = buf & startbit;
        SPI_SCLK = !spi->cpol;
        if (spi->cpha) SPI_MOSI = buf & startbit;
        SPI_SCLK = spi->cpol;
        startbit = spi->firstbit ? (startbit << 1) : (startbit >> 1);
    }
}

uint16_t spi_r(spi_t* spi) {
    uint8_t  i;
    uint16_t buf      = 0;
    uint16_t startbit = spi->firstbit ? 0x01 : (1 << spi->datasize);
    for (i = 0; i < spi->datasize; i++) {
        SPI_SCLK = !spi->cpol;
        if (!spi->cpha && SPI_MISO) buf |= startbit;
        SPI_SCLK = spi->cpol;
        if (spi->cpha && SPI_MISO) buf |= startbit;
        startbit = spi->firstbit ? (startbit << 1) : (startbit >> 1);
    }
    return buf;
}

uint16_t spi_rw(spi_t* spi, uint16_t send_data) {
    uint8_t  len       = spi->datasize;
    uint16_t recv_data = 0;
    uint16_t startbit  = spi->firstbit ? 0x01 : (1 << spi->datasize);
    while (len--) {
        if (spi->cpha)
            SPI_MOSI = send_data & startbit;
        else if (SPI_MISO)
            recv_data |= startbit;
        SPI_SCLK = !spi->cpol;
        if (!spi->cpha)
            SPI_MOSI = send_data & startbit;
        else if (SPI_MISO)
            recv_data |= startbit;
        SPI_SCLK = spi->cpol;
        startbit = spi->firstbit ? (startbit << 1) : (startbit >> 1);
    }
    return recv_data;
}
