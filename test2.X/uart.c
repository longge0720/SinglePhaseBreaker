#include "uart.h"
#include <stdio.h>
//使用串口时无法控制电机
void putch(unsigned char byte) 
{
 /* output one byte */
    TXREG = byte;
    while((TXSTA & (0x01<<1)) == 0);//发送缓冲区满，等待发送完成
}
static void PIC_ConfigUartGPIO(void)
{
    uint8_t tem;
    //APFCON： 备用引脚功能控制寄存器  引脚重映射方便布线
    APFCON &= ~(0x01 << 7);//RXD 映射到RC5
    APFCON &= ~(0x01 << 2);//TXD 映射到RC4
    
    TRISC &= ~(0x01 << 4);//RC4 为输出
    PORTC |= 0x01 << 4;
}
void PIC_ConfigUart(void)
{
    uint16_t tem;
    PIC_ConfigUartGPIO();
    //配置串口前取消串口使能位

    //TXSTA：发送状态和控制寄存器
    TXSTA &= ~(0x01 << 6);  //8位发送
    TXSTA |= (0x01 << 5);//使能发送
    TXSTA &= ~(0x01 << 4);//异步
    TXSTA |= (0x01 << 1);//异步高速 BRGH
    

    //BAUDCON：波特率控制寄存器
    BAUDCON &= ~(0x01 << 6);//自动波特率定时器未上溢
    BAUDCON &= ~(0x01 << 4);//发送不反向数据
    BAUDCON |= (0x01 << 3);//使用16位波特率发生器 BRG16
    BAUDCON &= ~(0x01 << 1);//接收器正常工作
    BAUDCON &= ~(0x01 << 0);//禁止自动波特率模式
    
    tem = 25;
    SPBRGH = tem >> 8;
    SPBRGL = (uint8_t )tem;    //十进制 832  32MHz 9600 波特率
    
    //RCSTA：接收状态和控制寄存器
    RCSTA &= ~(0x01 << 6);// 选择八位接收
    RCSTA &= ~(0x01 << 4);// 禁止接收器
    RCSTA &= ~(0x01 << 2);// 帧错误位
    RCSTA &= ~(0x01 << 1);// 溢出错误位
    RCSTA | = (0x01 << 7);//串口使能
    
    PIE1 &= ~(0x01 <<5);//禁止串口接收中断
    PIE1 &= ~(0x01 << 4);//禁止串口发送中断
}

void Uart_SendStr(uint8_t *str)
{
#if 0
    while(*str != '\0')
    {
         TXREG = *str++;
        while((TXSTA & (0x01<<1)) == 0);//发送缓冲区满，等待发送完成
    }
#endif
}

void Uart_SendDataLen(uint8_t *str,uint16_t len)
{
    while(len --)
    {
        TXREG = *str++;
        while((TXSTA & (0x01<<1)) == 0);//发送缓冲区满，等待发送完成
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