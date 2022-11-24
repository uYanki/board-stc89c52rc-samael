#include <REGX52.H>

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

void delay_ms(uint16_t ms) {
    uint16_t i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

void delay_10us(uint16_t ten_us) {
    while (ten_us--)
        ;
}

/***************** motor *****************/

#define left_front P0_0
#define left_back P0_1
#define right_front P0_2
#define right_back P0_3

typedef enum {
    forward,
    backward,
    left,
    right,
    stop,
} direction;

void car_move(direction dir /*方向*/) {
    switch (dir) {
        case forward:
            left_front  = 1;
            left_back   = 0;
            right_front = 1;
            right_back  = 0;
            break;
        case backward:
            left_front  = 0;
            left_back   = 1;
            right_front = 0;
            right_back  = 1;
            break;
        case left:
            left_front  = 0;
            left_back   = 1;
            right_front = 1;
            right_back  = 0;
            break;
        case right:
            left_front  = 1;
            left_back   = 0;
            right_front = 0;
            right_back  = 1;
            break;
        case stop:
        default:
            left_front  = 0;
            left_back   = 0;
            right_front = 0;
            right_back  = 0;
            break;
    }
}

/***************** ired *****************/

/* 遥控器码表
0x45 0x46 0x47
0x44 0x40 0x43
0x07 0x15 0x09
0x16 0x19 0x0D
0x0C 0x18 0x5E  (1 2 3)
0x08 0x1C 0x5A  (4 5 6)
0x42 0x52 0x4a  (7 8 9)
*/

#define IRED P3_2

uint8_t gired_data[4];  // gired_data[2] 为接收到的值

void ired_init(void) {
    IT0  = 1;  //下降沿触发
    EX0  = 1;  //打开中断0允许
    EA   = 1;  //打开总中断
    IRED = 1;  //初始化端口
}

void ired() interrupt 0  //外部中断0服务函数
{
    uint8_t  ired_high_time = 0;
    uint16_t time_cnt       = 0;
    uint8_t  i = 0, j = 0;

    if (IRED == 0) {
        time_cnt = 1000;
        while ((!IRED) && (time_cnt))  //等待引导信号9ms低电平结束,若超过10ms强制退出
        {
            delay_10us(1);  //延时约10us
            time_cnt--;
            if (time_cnt == 0) return;
        }
        if (IRED)  //引导信号9ms低电平已过,进入4.5ms高电平
        {
            time_cnt = 500;
            while (IRED && time_cnt)  //等待引导信号4.5ms高电平结束,若超过5ms强制退出
            {
                delay_10us(1);
                time_cnt--;
                if (time_cnt == 0) return;
            }
            for (i = 0; i < 4; i++)  //循环4次,读取4个字节数据
            {
                for (j = 0; j < 8; j++)  //循环8次读取每位数据即一个字节
                {
                    time_cnt = 600;
                    while ((IRED == 0) && time_cnt)  //等待数据1或0前面的0.56ms结束,若超过6ms强制退出
                    {
                        delay_10us(1);
                        time_cnt--;
                        if (time_cnt == 0) return;
                    }
                    time_cnt = 20;
                    while (IRED)  //等待数据1或0后面的高电平结束,若超过2ms强制退出
                    {
                        delay_10us(10);  //约0.1ms
                        ired_high_time++;
                        if (ired_high_time > 20) return;
                    }
                    gired_data[i] >>= 1;      //先读取的为低位,然后是高位
                    if (ired_high_time >= 8)  //如果高电平时间大于0.8ms,数据则为1,否则为0
                        gired_data[i] |= 0x80;
                    ired_high_time = 0;  //重新清零,等待下一次计算时间
                }
            }
        }
        if (gired_data[2] != ~gired_data[3])  //校验控制码与反码,错误则返回
        {
            for (i = 0; i < 4; i++)
                gired_data[i] = 0;
            return;
        }

        switch (gired_data[2]) {
            case 0x18: car_move(forward); break;
            case 0x52: car_move(backward); break;
            case 0x08: car_move(left); break;
            case 0x5A: car_move(right); break;
            default: car_move(stop); break;
        }
    }
}

/***************** main *****************/

void main() {
    ired_init();
    car_move(stop);
    while (1) {
    }
}
