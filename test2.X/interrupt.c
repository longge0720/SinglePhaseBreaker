#include "interrupt.h"
#include "define.h"
#include "keyboard.h"
void PIC_DisableInterrupt(void)
{
    //INTCON���жϿ��ƼĴ���
    INTCON &= ~(0x01 << 7);//��ֹȫ���ж�
    INTCON &= ~(0x01 << 6);//��ֹ�����ж�����λ
}
void PIC_EnableInterrupt(void)
{
    //INTCON���жϿ��ƼĴ���
    INTCON |= (0x01 << 7);//ʹ��ȫ���ж�
    INTCON |= (0x01 << 6);//�����ж�����λ
}

extern void Contrl_Signal(void);
//���õ��жϺ���
void InterruptTimer0(void)
{
    static unsigned long cnt = 0;
#if 1
 //   RA2 = 1;
    TMR0 = 5;//��ʱ������ֵ
    KeyScan(); //����ɨ��
    Contrl_Signal();//��ȡ�����ź�
    Motor_RunTimeCount();//ͳ�Ƶ������ʱ��
    Delay_Scan();
//    RA2 = 0;
#endif
}

//�ж��ܺ�������Ӳ��ֱ�Ӵ���
//void  __interrupt() ISR_Manger(void) 
void  interrupt ISR_Manger(void) 
{
    if(INTCON & (0x01 << 2)) //��ʱ��0 �ж�
    {   
        InterruptTimer0();
       INTCON &= ~(0x01 << 2);//�����ʱ���жϱ�־
    }
}

