#include "uart.h"
#include <stdio.h>
//ʹ�ô���ʱ�޷����Ƶ��
void putch(unsigned char byte) 
{
 /* output one byte */
    TXREG = byte;
    while((TXSTA & (0x01<<1)) == 0);//���ͻ����������ȴ��������
}
static void PIC_ConfigUartGPIO(void)
{
    uint8_t tem;
    //APFCON�� �������Ź��ܿ��ƼĴ���  ������ӳ�䷽�㲼��
    APFCON &= ~(0x01 << 7);//RXD ӳ�䵽RC5
    APFCON &= ~(0x01 << 2);//TXD ӳ�䵽RC4
    
    TRISC &= ~(0x01 << 4);//RC4 Ϊ���
    PORTC |= 0x01 << 4;
}
void PIC_ConfigUart(void)
{
    uint16_t tem;
    PIC_ConfigUartGPIO();
    //���ô���ǰȡ������ʹ��λ

    //TXSTA������״̬�Ϳ��ƼĴ���
    TXSTA &= ~(0x01 << 6);  //8λ����
    TXSTA |= (0x01 << 5);//ʹ�ܷ���
    TXSTA &= ~(0x01 << 4);//�첽
    TXSTA |= (0x01 << 1);//�첽���� BRGH
    

    //BAUDCON�������ʿ��ƼĴ���
    BAUDCON &= ~(0x01 << 6);//�Զ������ʶ�ʱ��δ����
    BAUDCON &= ~(0x01 << 4);//���Ͳ���������
    BAUDCON |= (0x01 << 3);//ʹ��16λ�����ʷ����� BRG16
    BAUDCON &= ~(0x01 << 1);//��������������
    BAUDCON &= ~(0x01 << 0);//��ֹ�Զ�������ģʽ
    
    tem = 25;
    SPBRGH = tem >> 8;
    SPBRGL = (uint8_t )tem;    //ʮ���� 832  32MHz 9600 ������
    
    //RCSTA������״̬�Ϳ��ƼĴ���
    RCSTA &= ~(0x01 << 6);// ѡ���λ����
    RCSTA &= ~(0x01 << 4);// ��ֹ������
    RCSTA &= ~(0x01 << 2);// ֡����λ
    RCSTA &= ~(0x01 << 1);// �������λ
    RCSTA | = (0x01 << 7);//����ʹ��
    
    PIE1 &= ~(0x01 <<5);//��ֹ���ڽ����ж�
    PIE1 &= ~(0x01 << 4);//��ֹ���ڷ����ж�
}

void Uart_SendStr(uint8_t *str)
{
#if 0
    while(*str != '\0')
    {
         TXREG = *str++;
        while((TXSTA & (0x01<<1)) == 0);//���ͻ����������ȴ��������
    }
#endif
}

void Uart_SendDataLen(uint8_t *str,uint16_t len)
{
    while(len --)
    {
        TXREG = *str++;
        while((TXSTA & (0x01<<1)) == 0);//���ͻ����������ȴ��������
    }
}


void Uart_ShowInt(uint16_t intnum)
{
#if 0
    uint8_t buf[6];
    
    buf[0] = intnum/1000 + '0'; 
    intnum %= 1000;
    buf[1] = intnum/100 + '0';
    intnum %= 100;
    buf[2] = intnum/10 + '0';
    intnum %= 10;
    buf[3] = intnum + '0';
    buf[4] = ' ';
    buf[5] = 0;
    Uart_SendStr(buf);
#endif
}