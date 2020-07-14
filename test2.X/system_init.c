#include "system_init.h"

//所有GPIO口配置汇总
//RA0: ICSPDAT                   ICSP串行调试接口
//RA1: ICSPCLK                   ICSP串行调试接口
//RA2: 电机输出                    
//RA3: Vpp编程电压,/MCLR主复位     ICSP串行调试接口
//RA4:（霍尔输入）                 CLOCKOUT  FOSC/4 用来测试系统时钟频率
//RA5: 霍尔输入          

//RC0: 电机输出
//RC1: 按键，上拉输入             
//RC2: 输出，控制MOS
//RC3: 
//RC4：(霍尔输入)                 串口 TX
//RC5：（控制信号输入)            串口 RX

//配置霍尔引脚为上拉
void PIC_ConfigHallGPIO(void)
{
    
}
//用于配置开关量
void PIC_ConfigGPIO(void)
{
//RC1为控制信号引脚
    TRISC   |= 0x01 << 1;
    ANSELC  &= ~(0x01 << 1);//数字IO
    //PORTC   |= 0x01 << 1;//锁存1
#if 0
//RC0 为是否有相线电压
    TRISC   |= 0x01 << 0;
    ANSELC  &= ~(0x01 << 0);//数字IO
    //PORTC   |= 0x01 << 0;//锁存1
#endif
    //RA2输入模式
    TRISA   |= 0x01 << 2;
    ANSELA  &= ~(0x01 << 2);//数字IO
}

void PIC_ConfigClock(void)
{
    //OSCCON |= (0x01 << 7);//SPLLEN :如果配置字 1 中的 PLLEN = 1,SPLLEN 位被忽略。总是使能 4x PLL （受振荡器要求制约）软件 PLL 使能位
    SPLLEN  = 0;    //禁止系统倍频
    OSCCON &= ~(0x0f << 3);//IRCF<3:0>:振荡器控制寄存器，将配置位清空
   // OSCCON |= (0x0e << 3);//内部晶振配置为8M/32M
    
    OSCCON |= (0x0b << 3);//内部晶振配置为1MHz
    
    
    OSCCON &= ~(0x03);//SCS<1:0>
      
}
//配置时钟频率为32Khz，用于低功耗处理
void PIC_ConfigClock32Khz(void)
{
    OSCCON &= ~(0x0f << 3);//IRCF<3:0>:振荡器控制寄存器，将配置位清空
    NOP();
    NOP();
    OSCCON |= (0x00 << 3);//内部晶振配置为32Khz
    NOP();
    NOP();
}

//配置配置寄存器，再初始化时钟之后调用
void PIC_ConfigOptionReg(void)
{

  //  OPTION_REG = 0;             //配置寄存器清0
  //  OPTION_REG &= ~(0x01 << 7);//通过各个 WPUx 锁存值使能弱上拉
    OPTION_REG |= (0x01 << 7);//禁止弱上拉电阻
    
    OPTION_REG &= ~(0x01 << 6);//0 = RA2/INT 引脚的下降沿触发中断
    OPTION_REG &= ~(0x01 << 5);//0 = 内部指令周期时钟 （FOSC/4） 定时器0时钟选择
    OPTION_REG &= ~(0x01 << 4);//0 = T0CKI 引脚信号从低至高跳变时，递增计数 定时器时钟边沿选择
    OPTION_REG &= ~(0x01 << 3);//0 = 预分频器分配给 Timer0 模块

    

    OPTION_REG &= ~(0x07 << 0);//PS<2:0>： 预分频比选择位  定时器预分频选择
    OPTION_REG |= (0x04 << 0);

}
void System_Init(void)
{
    PIC_ConfigClock();
    PIC_ConfigOptionReg();
}

//看门狗功能还未实现
void WatchDog_Init(void)    //看门狗初始化，初始化看门狗复位时间
{
    WDTCON = 0x1b;//开启看门狗使能位，设置超时时间为8S
}

void WatchDog_Reset(void)
{
    WDTCON = 0;
    WDTCON |= 0x01;
    while(1);//等待看门狗复位
}

void Feed_WatchDog(void)    //喂狗，重置看门狗定时器
{
    CLRWDT();
}
void PIC_CofnigTimer0(void)
{
#if 0
    OPTION_REG &= ~(0x01 << 3);//使用预分频器
    
    OPTION_REG &= ~(0x07 << 0);//定时器0 预分频系数为64
    OPTION_REG |= (0x05 << 0);
    
    OPTION_REG &= ~(0x01 << 5);//时钟为fosc/4
#endif
    OPTION_REG |= (0x01 << 3);//不适用预分频
    OPTION_REG &= ~(0x01 << 5);//时钟为fosc/4
    
    TMR0 = 5;//0xff - TMR0为溢出值
    
    INTCON &= ~(0x01 << 2);//清除定时器0 中断标志
    INTCON |= (0x01 << 5);//使能定时器0中断
}
