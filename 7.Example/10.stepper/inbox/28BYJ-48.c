//**** ���� ********************************************************************
/*******************************************************************************
 * �������Ի�����Դ�����������д�����ȸ�Ƽ����޹�˾�ռ�
 * �����û��ο�ѧϰ������˾���ṩ�κμ���֧��
 * ����������Բο�������Ӧ����ʵ�ʹ����У���һ����ͨ������
 * ��˾��վ http://www.szdx-smart.com/
 * �Ա���ַ https://shop184598174.taobao.com/?spm=a1z10.5-c-s.w12096189-21564973333.3.547b1176WCCDxR&scene=taobao_shop
 *******************************************************************************/
/********************************************************************
* �ļ���  ��  ����ͨ����Ƭ�����Ʋ������
* ����    :  ���ļ�ʵ��ͨ������BT04-E02����Ƭ�������ݿ��Ʋ��������

***********************************************************************/
/*
 Created: 2022/5/31
 Author:  zhong
*/

#include <reg52.h>
#define uint  unsigned int
#define uchar unsigned char
uint i, j, k;
uint N = 128;                                                                 // ������ת�������ã�����D=N*45/���ٱȣ�����1:16���ٱȵĲ����������ת������ΪD=128*45/16=360��

uchar code single_pos[4] = {0x07, 0x0b, 0x0d, 0x0e};                          // ������������ʽ��ת�� D-C-B-A
uchar code single_rev[4] = {0x0e, 0x0d, 0x0b, 0x07};                          // ������������ʽ��ת�� A-B-C-D
uchar code double_pos[4] = {0x06, 0x03, 0x09, 0x0c};                          // ˫����������ʽ��ת�� AD-DC-CB-BA
uchar code double_rev[4] = {0x0c, 0x09, 0x03, 0x06};                          // ˫����������ʽ��ת�� AB-BC-CD-DA
uchar code eight_pos[8]  = {0x06, 0x07, 0x03, 0x0b, 0x09, 0x0d, 0x0c, 0x0e};  // ����������ʽ��ת�� AD-D-DC-C-CB-B-BA-A
uchar code eight_rev[8]  = {0x0e, 0x0c, 0x0d, 0x09, 0x0b, 0x03, 0x07, 0x06};  // ����������ʽ��ת�� A-AB-B-BC-C-CD-D-DA

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

/*//������ʱ����
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

        /*m_single_pos();//������������ʽ��ת360��
        delay(200);
        m_single_rev();//������������ʽ��ת360��
        delay(200);
        m_double_pos();//˫����������ʽ��ת360��
        delay(200);
        m_double_rev();//˫����������ʽ��ת360��
        delay(200);
        m_eight_pos();//����������ʽ��ת360��
        delay(200);
        m_eight_rev();//����������ʽ��ת360��
        delay(200);*/
    }
}

void UsartConfiguration()
{
    /*SCON=0X50;                        //����Ϊ������ʽ1
    TMOD=0X20;                        //���ü�����������ʽ2
    PCON=0X80;                        //�����ʼӱ�
    TH1=0XF3;                         //��������ʼֵ���ã�ע�Ⲩ������4800��(����Ϊ12.0MHz)
    TL1=0XF3;
    ES=1;                             //�򿪽����ж�
    EA=1;                             //�����ж�
    TR1=1;                            //�򿪼�����1*/
    IE   = 0x90;  // �ж�����Ĵ���
    SCON = 0x50;  // SCON=0x50��SM1=1��REN=1 ���ô��ڵ���ʽ1�������ڽ��գ�һ�����TMOD=0x20�������жϳ����ʼ��
    TMOD = 0x21;  //
    TH1  = 0xf3;  // ���ò�����Ϊ2400
    TL1  = 0xf3;  //
    TCON = 0x40;  // ��ʱ����ֵ �򿪴���

    TR1 = 1;      // �򿪶�ʱ��1
    ES  = 1;      //
    EA  = 1;      // �������ж�
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

void Usart() interrupt 4  // ���п��жϣ�Ϊʲô��4���Բο���һƪ�жϵ�����
{
    receiveData = SBUF;   // ��ȥ���յ�������
    RI          = 0;      // ��������жϱ�־λ
    // SBUF=receiveData;                      //�����յ������ݷ��뵽���ͼĴ���
}

/********************��ʱz����**************************************/
void delay(uint z)
{
    uint x, y;
    for (x = z; x > 0; x--)
        for (y = 110; y > 0; y--)
            ;
}

/*******************������������ת(N*45/16)��***********************/
void m_single_pos()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // �ڵ����Ĺ�����ʽ�£�һ������ת��ת���Ƕ�Ϊ5.625*2=11.25�ȣ����Ĺ�45��
        {
            P1 = single_pos[j];
            delay(5);  // �ʵ���ʱ����֤ת��ת��ʱ�䣬��ʱ���̻ᶪ��
            j++;
        }
    }
}

/*******************������������ת(N*45/16��)***********************/
void m_single_rev()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // �ڵ����Ĺ�����ʽ�£�һ������ת��ת���Ƕ�Ϊ5.625*2=11.25�ȣ����Ĺ�45��
        {
            P1 = single_rev[j];
            delay(5);
            j++;
        }
    }
}

/*******************˫����������ת(N*45/16)��***********************/
void m_double_pos()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // ��˫���Ĺ�����ʽ�£�һ������ת��ת���Ƕ�Ϊ5.625*2=11.25�ȣ����Ĺ�45��
        {
            P1 = double_pos[j];
            delay(8);
            j++;
        }
    }
}

/*******************˫����������ת(N*45/16)��***********************/
void m_double_rev()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 4; i++)  // ��˫���Ĺ�����ʽ�£�һ������ת��ת���Ƕ�Ϊ5.625*2=11.25�ȣ����Ĺ�45��
        {
            P1 = double_rev[j];
            delay(8);
            j++;
        }
    }
}

/*******************����������ת(N*45/16)��***********************/
void m_eight_pos()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 8; i++)  // �ڰ��Ĺ�����ʽ�£�һ������ת��ת���Ƕ�Ϊ5.625�ȣ����Ĺ�45��
        {
            P1 = eight_pos[j];
            delay(2);
            j++;
        }
    }
}

/*******************����������ת(N*45/16)��***********************/
void m_eight_rev()
{
    for (k = 0; k < N; k++) {
        j = 0;
        for (i = 0; i < 8; i++)  // �ڰ��Ĺ�����ʽ�£�һ������ת��ת���Ƕ�Ϊ5.625�ȣ����Ĺ�45��
        {
            P1 = eight_rev[j];
            delay(2);
            j++;
        }
    }
}
