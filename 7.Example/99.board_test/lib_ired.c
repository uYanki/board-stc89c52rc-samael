#include "lib_ired.h"

void ired_init(void) {
    IT3  = 1;  // 下降沿触发
    EX3  = 1;  // 中断3开关
    IRED = 1;  // 引脚默认电平
}

uint8_t gired_data[4];
void    ired_isr() interrupt IRED_ISR  //外部中断3服务函数
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

        ired_handle(gired_data[2]);  // gired_data[2] 为接收到的值
    }
}
