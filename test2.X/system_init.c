#include "system_init.h"

//����GPIO�����û���
//RA0: ICSPDAT                   ICSP���е��Խӿ�
//RA1: ICSPCLK                   ICSP���е��Խӿ�
//RA2: ������                    
//RA3: Vpp��̵�ѹ,/MCLR����λ     ICSP���е��Խӿ�
//RA4:���������룩                 CLOCKOUT  FOSC/4 ��������ϵͳʱ��Ƶ��
//RA5: ��������          

//RC0: ������
//RC1: ��������������             
//RC2: ���������MOS
//RC3: 
//RC4��(��������)                 ���� TX
//RC5���������ź�����)            ���� RX

//���û�������Ϊ����
void PIC_ConfigHallGPIO(void)
{
    
}
//�������ÿ�����
void PIC_ConfigGPIO(void)
{
//RC1Ϊ�����ź�����
    TRISC   |= 0x01 << 1;
    ANSELC  &= ~(0x01 << 1);//����IO
    //PORTC   |= 0x01 << 1;//����1
#if 0
//RC0 Ϊ�Ƿ������ߵ�ѹ
    TRISC   |= 0x01 << 0;
    ANSELC  &= ~(0x01 << 0);//����IO
    //PORTC   |= 0x01 << 0;//����1
#endif
    //RA2����ģʽ
    TRISA   |= 0x01 << 2;
    ANSELA  &= ~(0x01 << 2);//����IO
}

void PIC_ConfigClock(void)
{
    //OSCCON |= (0x01 << 7);//SPLLEN :��������� 1 �е� PLLEN = 1,SPLLEN λ�����ԡ�����ʹ�� 4x PLL ��������Ҫ����Լ����� PLL ʹ��λ
    SPLLEN  = 0;    //��ֹϵͳ��Ƶ
    OSCCON &= ~(0x0f << 3);//IRCF<3:0>:�������ƼĴ�����������λ���
   // OSCCON |= (0x0e << 3);//�ڲ���������Ϊ8M/32M
    
    OSCCON |= (0x0b << 3);//�ڲ���������Ϊ1MHz
    
    
    OSCCON &= ~(0x03);//SCS<1:0>
      
}
//����ʱ��Ƶ��Ϊ32Khz�����ڵ͹��Ĵ���
void PIC_ConfigClock32Khz(void)
{
    OSCCON &= ~(0x0f << 3);//IRCF<3:0>:�������ƼĴ�����������λ���
    NOP();
    NOP();
    OSCCON |= (0x00 << 3);//�ڲ���������Ϊ32Khz
    NOP();
    NOP();
}

//�������üĴ������ٳ�ʼ��ʱ��֮�����
void PIC_ConfigOptionReg(void)
{

  //  OPTION_REG = 0;             //���üĴ�����0
  //  OPTION_REG &= ~(0x01 << 7);//ͨ������ WPUx ����ֵʹ��������
    OPTION_REG |= (0x01 << 7);//��ֹ����������
    
    OPTION_REG &= ~(0x01 << 6);//0 = RA2/INT ���ŵ��½��ش����ж�
    OPTION_REG &= ~(0x01 << 5);//0 = �ڲ�ָ������ʱ�� ��FOSC/4�� ��ʱ��0ʱ��ѡ��
    OPTION_REG &= ~(0x01 << 4);//0 = T0CKI �����źŴӵ���������ʱ���������� ��ʱ��ʱ�ӱ���ѡ��
    OPTION_REG &= ~(0x01 << 3);//0 = Ԥ��Ƶ������� Timer0 ģ��

    

    OPTION_REG &= ~(0x07 << 0);//PS<2:0>�� Ԥ��Ƶ��ѡ��λ  ��ʱ��Ԥ��Ƶѡ��
    OPTION_REG |= (0x04 << 0);

}
void System_Init(void)
{
    PIC_ConfigClock();
    PIC_ConfigOptionReg();
}

//���Ź����ܻ�δʵ��
void WatchDog_Init(void)    //���Ź���ʼ������ʼ�����Ź���λʱ��
{
    WDTCON = 0x1b;//�������Ź�ʹ��λ�����ó�ʱʱ��Ϊ8S
}

void WatchDog_Reset(void)
{
    WDTCON = 0;
    WDTCON |= 0x01;
    while(1);//�ȴ����Ź���λ
}

void Feed_WatchDog(void)    //ι�������ÿ��Ź���ʱ��
{
    CLRWDT();
}
void PIC_CofnigTimer0(void)
{
#if 0
    OPTION_REG &= ~(0x01 << 3);//ʹ��Ԥ��Ƶ��
    
    OPTION_REG &= ~(0x07 << 0);//��ʱ��0 Ԥ��Ƶϵ��Ϊ64
    OPTION_REG |= (0x05 << 0);
    
    OPTION_REG &= ~(0x01 << 5);//ʱ��Ϊfosc/4
#endif
    OPTION_REG |= (0x01 << 3);//������Ԥ��Ƶ
    OPTION_REG &= ~(0x01 << 5);//ʱ��Ϊfosc/4
    
    TMR0 = 5;//0xff - TMR0Ϊ���ֵ
    
    INTCON &= ~(0x01 << 2);//�����ʱ��0 �жϱ�־
    INTCON |= (0x01 << 5);//ʹ�ܶ�ʱ��0�ж�
}
