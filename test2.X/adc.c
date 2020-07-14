#include "adc.h"

//�����õ�ADC���ų�ʼ��
void ADC_AllPin_Init(void)
{
    //RC1       �����ź���������ˣ�ADCͨ��5
#if 0
    TRISC   |= (0x01 << 1);
    ANSELC  |= (0x01 << 1);//ģ������
    WPUC    &= ~(0x01 << 1);//��ֹ������
#endif
    //RC0,ͨ��1
     TRISC |= (0x01 << 0);
     ANSELC |= (0x01 << 0);
     WPUC &= ~(0x01 << 0);
#if 0
     //RA2ͨ��
    TRISA |= (0x01 << 2);
    ANSELA |= (0x01 << 2);
    WPUA &= ~(0x01 << 2);
#endif
}

void ADC_Init(void)
{
    ADC_AllPin_Init();
    
    ADCON1 |= (0x01 << 7);//�Ҷ���
    ADCON1 &= ~(0x07 << 4);//���ADCʱ��ѡ��
    //ADCON1 |= (0x06 << 4);//ADC ʱ��ΪFOSC/64
    ADCON1 |= (0x00 << 4);//ADC ʱ��ΪFOSC/2
    ADCON1 &= ~(0x03 << 0);//Vref+ ���ӵ�AVDD
    
    ADCON0  |= 0x01;//ʹ��ADC
}
//32M /64 ת��ʱ��Ϊ6us
uint16_t Get_ADCValue(ADCChnel_Enumdef channel)
{
    uint16_t tem = 0;
    ADCON0 &= ~(0x1f << 2);     //���Ƶ��ѡ��λ
    ADCON0 |= (channel << 2);
    ADCON0 |= 0x01 << 1;//����ADCת������
    
    while(ADCON0 & 0x02 );//�ȴ�ADCת������
    
    tem = (ADRESH  << 8) | ADRESL;
    return tem;
}