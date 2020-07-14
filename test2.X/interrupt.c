#include "interrupt.h"
#include "define.h"
#include "keyboard.h"
void PIC_DisableInterrupt(void)
{
    //INTCON：中断控制寄存器
    INTCON &= ~(0x01 << 7);//禁止全局中断
    INTCON &= ~(0x01 << 6);//禁止外设中断允许位
}
void PIC_EnableInterrupt(void)
{
    //INTCON：中断控制寄存器
    INTCON |= (0x01 << 7);//使能全局中断
    INTCON |= (0x01 << 6);//外设中断允许位
}

extern void Contrl_Signal(void);
//调用的中断函数
void InterruptTimer0(void)
{
    static unsigned long cnt = 0;
#if 1
 //   RA2 = 1;
    TMR0 = 5;//定时器赋初值
    KeyScan(); //按键扫描
    Contrl_Signal();//获取控制信号
    Motor_RunTimeCount();//统计电机运行时间
    Delay_Scan();
//    RA2 = 0;
#endif
}

//中断总函数，由硬件直接触发
//void  __interrupt() ISR_Manger(void) 
void  interrupt ISR_Manger(void) 
{
    if(INTCON & (0x01 << 2)) //定时器0 中断
    {   
        InterruptTimer0();
       INTCON &= ~(0x01 << 2);//清除定时器中断标志
    }
}

