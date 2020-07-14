#include "adc.h"

//所有用到ADC引脚初始化
void ADC_AllPin_Init(void)
{
    //RC1       控制信号引脚输入端，ADC通道5
#if 0
    TRISC   |= (0x01 << 1);
    ANSELC  |= (0x01 << 1);//模拟输入
    WPUC    &= ~(0x01 << 1);//禁止弱上拉
#endif
    //RC0,通道1
     TRISC |= (0x01 << 0);
     ANSELC |= (0x01 << 0);
     WPUC &= ~(0x01 << 0);
#if 0
     //RA2通道
    TRISA |= (0x01 << 2);
    ANSELA |= (0x01 << 2);
    WPUA &= ~(0x01 << 2);
#endif
}

void ADC_Init(void)
{
    ADC_AllPin_Init();
    
    ADCON1 |= (0x01 << 7);//右对齐
    ADCON1 &= ~(0x07 << 4);//清空ADC时钟选择
    //ADCON1 |= (0x06 << 4);//ADC 时钟为FOSC/64
    ADCON1 |= (0x00 << 4);//ADC 时钟为FOSC/2
    ADCON1 &= ~(0x03 << 0);//Vref+ 连接到AVDD
    
    ADCON0  |= 0x01;//使能ADC
}
//32M /64 转换时间为6us
uint16_t Get_ADCValue(ADCChnel_Enumdef channel)
{
    uint16_t tem = 0;
    ADCON0 &= ~(0x1f << 2);     //清空频道选择位
    ADCON0 |= (channel << 2);
    ADCON0 |= 0x01 << 1;//启动ADC转换周期
    
    while(ADCON0 & 0x02 );//等待ADC转换结束
    
    tem = (ADRESH  << 8) | ADRESL;
    return tem;
}