//**** 声明 ********************************************************************
/*******************************************************************************
 * 下面来自互联开源程序，由深圳市大夏龙雀科技有限公司收集
 * 方便用户参考学习，本公司不提供任何技术支持
 * 程序仅供测试参考，不能应用在实际工程中，不一定能通过编译
 * 公司网站 http://www.szdx-smart.com/
 * 淘宝网址 https://shop184598174.taobao.com/?spm=a1z10.5-c-s.w12096189-21564973333.3.547b1176WCCDxR&scene=taobao_shop
 *******************************************************************************/
/********************************************************************
* 文件名  ：  蓝牙通过单片机控制步进电机
* 描述    :  该文件实现通过蓝牙BT04-E02给单片机发数据控制步进电机。

***********************************************************************/
/*
 Created: 2022/5/31
 Author:  zhong
*/

#include <reg52.h>
#define uint  unsigned int
#define uchar unsigned char
uint i, j, k;
uint N = 128;                                                                 // 主轴旋转度数设置，度数D=N*45/减速比，采用1:16减速比的步进电机，旋转度数即为D=128*45/16=360度

uchar code single_pos[4] = {0x07, 0x0b, 0x0d, 0x0e};                          // 单四拍驱动方式正转表 D-C-B-A
uchar code single_rev[4] = {0x0e, 0x0d, 0x0b, 0x07};                          // 单四拍驱动方式反转表 A-B-C-D
uchar code double_pos[4] = {0x06, 0x03, 0x09, 0x0c};                          // 双四拍驱动方式正转表 AD-DC-CB-BA
uchar code double_rev[4] = {0x0c, 0x09, 0x03, 0x06};                          // 双四拍驱动方式反转表 AB-BC-CD-DA
uchar code eight_pos[8]  = {0x06, 0x07, 0x03, 0x0b, 0x09, 0x0d, 0x0c, 0x0e};  // 八拍驱动方式正转表 AD-D-DC-C-CB-B-BA-A
uchar code eight_rev[8]  = {0x0e, 0x0c, 0x0d, 0x09, 0x0b, 0x03, 0x07, 0x06};  // 八拍驱动方式反转表 A-AB-B-BC-C-CD-D-DA

void delay(uint z);
void m_single_pos();
void m_single_rev();
void m_double_pos();
void m_double_rev();
void m_eight_pos();
void m_eight_rev();

sbit IN1 = P1 ^ 0;
sbit IN2 = P1 ^ 1;
sbit ENA = P1 ^ 3;

#define uchar unsigned char
uchar receiveData;

void delay_us(int a)
{
    while (a--)
        ;
}

/*//简易延时函数
void delay(uint xms)
{
    for(xms;xms>0;xms--);
}*/

void UsartConfiguration();
void bbb();

void main()
{
    UsartConfiguration();

    TI  = 0;
    ENA = 0;
    IN1 = 0;
    IN2 = 0;
    while (1) {
        if (receiveData == '1') {
            ENA = 1;
            IN1 = 1;
            IN2 = 0;

        } else if (receiveData == '2') {
            ENA = 1;
            IN1 = 0;
            IN2 = 1;
        } else if (receiveData == '3') {
            ENA = 0;
            IN1 = 1;
            IN2 = 1;
        } else if (receiveData == '4') {
            ENA = 1;
            IN1 = 1;
            IN2 = 0;
            delay(200);
            IN1 = 1;
            IN2 = 1;
            delay(200);
            IN1 = 0;
            IN2 = 1;
            delay(200);

            IN1 = 0;
            IN2 = 0;
            delay(200);
        } else if (receiveData == '0') {
            receiveData = 6;
            bbb();
        }

        /*m_single_pos();//单四拍驱动方式正转360度
        delay(200);
        m_single_rev();//单四拍驱动方式反转360度
        delay(200);
        m_double_pos();//双四拍驱动方式正转360度
        delay(200);
        m_double_rev();//双四拍驱动方式反转360度
        delay(200);
        m_eight_pos();//八拍驱动方式正转360度
        delay(200);
        m_eight_rev();//八拍驱动方式反转360度
        delay(200);*/
    }
}

void UsartConfiguration()
{
    /*SCON=0X50;                        //设置为工作方式1
    TMOD=0X20;                        //设置计数器工作方式2
    PCON=0X80;                        //波特率加倍
    TH1=0XF3;                         //计数器初始值设置，注意波特率是4800的(晶振为12.0MHz)
    TL1=0XF3;
    ES=1;                             //打开接收中断
    EA=1;                             //打开总中断
    TR1=1;                            //打开计数器1*/
    IE   = 0x90;  // 中断允许寄存器
    SCON = 0x50;  // SCON=0x50是SM1=1和REN=1 设置串口道方式1并允许串口接收，一般配合TMOD=0x20做串口中断程序初始化
    TMOD = 0x21;  //
    TH1  = 0xf3;  // 设置波特率为2400
    TL1  = 0xf3;  //
    TCON = 0x40;  // 定时器赋值 打开串口

    TR1 = 1;      // 打开定时器1
    ES  = 1;      //
    EA  = 1;      // 打开整个中断
    TI  = 1;
}

void bbb()
{
    int i;

    IN1 = 1;
    IN2 = 0;
    for (i = 200; i >= 0; i--) {
        ENA = 1;
        delay(i);
        ENA = 0;
        delay(200 - i);

        if (receiveData == '3') {
            ENA = 0;
            IN1 = 1;
            IN2 = 1;
            return;
        }
    }

    IN1 = 1;
    IN2 = 1;
    // delay(200);
}

void Usart() interrupt 4  // 串行口中断，为什么是4可以参考上一篇中断的文章
{
    receiveData = SBUF;   // 出去接收到的数据
    RI          = 0;      // 清除接收中断标志位
    // SBUF=receiveData;                      //将接收到的数据放入到发送寄存器
}

/********************延时z毫秒**************************************/
void delay(uint z)
{
    uint x, y;
    for (x = z; x > 0; x--)
        for (y = 110; y > 0; y--)
            ;
}

/*******************单四拍驱动正转(N*45/16)度***********************/
void m_single_pos()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // 在单四拍工作方式下，一个脉冲转子转动角度为5.625*2=11.25度，四拍共45度
        {
            P1 = single_pos[j];
            delay(5);  // 适当延时，保证转子转动时间，延时过短会丢拍
            j++;
        }
    }
}

/*******************单四拍驱动反转(N*45/16度)***********************/
void m_single_rev()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // 在单四拍工作方式下，一个脉冲转子转动角度为5.625*2=11.25度，四拍共45度
        {
            P1 = single_rev[j];
            delay(5);
            j++;
        }
    }
}

/*******************双四拍驱动正转(N*45/16)度***********************/
void m_double_pos()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // 在双四拍工作方式下，一个脉冲转子转动角度为5.625*2=11.25度，四拍共45度
        {
            P1 = double_pos[j];
            delay(8);
            j++;
        }
    }
}

/*******************双四拍驱动反转(N*45/16)度***********************/
void m_double_rev()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // 在双四拍工作方式下，一个脉冲转子转动角度为5.625*2=11.25度，四拍共45度
        {
            P1 = double_rev[j];
            delay(8);
            j++;
        }
    }
}

/*******************八拍驱动正转(N*45/16)度***********************/
void m_eight_pos()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 8; i++)  // 在八拍工作方式下，一个脉冲转子转动角度为5.625度，八拍共45度
        {
            P1 = eight_pos[j];
            delay(2);
            j++;
        }
    }
}

/*******************八拍驱动反转(N*45/16)度***********************/
void m_eight_rev()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 8; i++)  // 在八拍工作方式下，一个脉冲转子转动角度为5.625度，八拍共45度
        {
            P1 = eight_rev[j];
            delay(2);
            j++;
        }
    }
}
