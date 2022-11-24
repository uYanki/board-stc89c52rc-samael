#include "lib_i2c.h"

/* I2C

空闲状态: 时钟线和数据线均为高电平
起始信号: 时钟线为高电平时,数据线由高电平变为低电平
停止信号: 时钟线为高电平时,数据线由低电平变为高电平
数据/应答位 单比特发送: 主机在时钟线为低电平时,可改变数据线电平
数据/应答位 单比特接收: 从机在时钟线为高电平时,对数据线进行采样
发送数据: 发送方向接受方发送1个字节(8个比特),然后接收方向发送方发送1个应答位(1个比特)
接收数据: 接收方从发送方接收1个字节(8个比特),然后发送方从接受方接收1个应答位(1个比特)
应答位: 在一方发送/接收8个比特后,另一方需发送/接收1个应答位 (低电平0为应答,高电平1为不应答)
延时作用: 防止设备在时钟线的上升沿/下降沿时进行采样

START: when CLK is high, buf change form high to low
STOP:  when CLK is high buf change form low to high

注: 整个收发过程中时钟线都由主机控制,且先发送高位再发送低位

https://blog.csdn.net/CCSGTC/article/details/108571947

主机往从机写数据:
1.主机控制数据线,产生起始条件,开始发送
2.主机控制数据线,发送7位地址位(A7:A1)+1位读写位(R/W,这里置0)
  读写位为最低位,且高电平代表读,低电平代表写
3.从机控制数据线,发送应答位
4.主机控制数据线,发送从机中8位寄存器地址
5.从机控制数据线,发送应答位
6.主机控制数据线,发送要写的8位数据
7.从机控制数据线,发送应答位
8.可重复4~7的步骤,连续发送数据
9.主机控制数据线,产生停止条件,停止发送

主机往从机读数据:
1.主机控制数据线,产生起始条件,开始发送
2.主机控制数据线,发送7位地址位(A7:A1)+1位读写位(R/W,这里置0)
  读写位为最低位,且高电平代表读,低电平代表写
3.从机控制数据线,发送应答位
4.主机控制数据线,发送从机中8位寄存器地址
5.从机控制数据线,发送应答位 // 前5步和上方的相同
6.主机控制数据线,重新产生起始条件
7.主机控制数据线,发送7位地址位(A7:A1)+1位读写位(R/W,这里置1)
8.从机控制数据线,发送应答位
9.主机控制数据线,发送8位数据
10.从机控制数据线,发送应答位;如要发送停止条件,需先发送不应答

*/

void i2c_delay() {
    uint8_t i;
    for (i = 0; i < 10; ++i) {}
}

void i2c_start() {
    I2C_SDA = 1;
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 0;
}

void i2c_stop() {
    I2C_SDA = 0;
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 1;
}

void i2c_send_byte(uint8_t byte) {
    uint8_t i = 8;
    while (i--) {
        I2C_SCL = 0;
        I2C_SDA = byte & (1 << i);
        i2c_delay();
        I2C_SCL = 1;
    }
}

uint8_t i2c_recv_byte() {
    uint8_t i    = 8;
    uint8_t byte = 0;
    while (i--) {
        I2C_SCL = 1;
        if (I2C_SDA)  //
            byte |= (1 << i);
        i2c_delay();
        I2C_SCL = 0;
    }
    return byte;
}

void i2c_send_ack() {
    I2C_SCL = 0;
    I2C_SDA = 0;
    I2C_SCL = 1;
}

void i2c_send_nack() {
    I2C_SCL = 0;
    I2C_SDA = 1;
    i2c_delay();
    I2C_SCL = 1;
}

uint8_t i2c_recv_ack() {
    uint8_t t = 0;
    I2C_SCL   = 0;
    I2C_SDA   = 1;
    i2c_delay();
    I2C_SCL = 1;
    while (I2C_SDA) {
        if (++t > 250) {
            i2c_stop();
            return 1;  //无应答
        }
    }
    I2C_SCL = 0;
    return 0;  //有应答
}

// 检测器件是否存在 0不存在 1存在
uint8_t i2c_check(uint8_t addr_dev) {
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 0);
    if (i2c_recv_ack())  //
        return 0;
    i2c_stop();
    return 1;
}

uint8_t i2c_write_byte(uint8_t addr_dev, uint8_t addr_reg, uint8_t byte) {
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 0);
    if (i2c_recv_ack()) return 0;
    i2c_send_byte(addr_reg);
    if (i2c_recv_ack()) return 0;
    i2c_send_byte(byte);
    if (i2c_recv_ack()) return 0;
    i2c_stop();
    return 1;
}

uint8_t i2c_read_byte(uint8_t addr_dev, uint8_t addr_reg, uint8_t* byte) {
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 0);
    if (i2c_recv_ack()) return 0;
    i2c_send_byte(addr_reg);
    if (i2c_recv_ack()) return 0;
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 1);
    if (i2c_recv_ack()) return 0;
    *byte = i2c_recv_byte();
    i2c_send_nack();
    i2c_stop();
    return 1;
}

uint8_t i2c_write_bytes(uint8_t addr_dev, uint8_t addr_reg, uint8_t* buf, uint8_t len) {
    uint8_t i;
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 0);
    if (i2c_recv_ack()) return 0;
    i2c_send_byte(addr_reg);
    if (i2c_recv_ack()) return 0;
    for (i = 0; i < len; ++i) {
        i2c_send_byte(buf[i]);
        if (i2c_recv_ack()) return 0;
    }
    i2c_stop();
    return 1;
}

uint8_t i2c_read_bytes(uint8_t addr_dev, uint8_t addr_reg, uint8_t* buf, uint8_t len) {
    uint8_t i;
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 0);
    if (i2c_recv_ack()) return 0;
    i2c_send_byte(addr_reg);
    if (i2c_recv_ack()) return 0;
    i2c_start();
    i2c_send_byte((addr_dev << 1) | 1);
    if (i2c_recv_ack()) return 0;
    for (i = 0; i < len - 1; ++i) {
        *buf++ = i2c_recv_byte();
        i2c_send_ack();
    }
    *buf = i2c_recv_byte();
    i2c_send_nack();
    i2c_stop();
    return 1;
}
