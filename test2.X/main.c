/* 
 * File:   main.c
 * Author: USER
 *
 * Created on 2019年3月13日, 下午3:16
 */

#include "define.h"
#include "config.h"
#include "uart.h"
#include <stdio.h>
#include "motor.h"
#include "internal_e2prom.h"
#include <htc.h>


//__CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_ON & FCMEN_ON);
//__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & BORV_19 & LVP_ON);
/*控制信号为0.5V ~ 1.5V 信号 V1.0版本*/
//#ifdef      PIC_DEBUG_VERSION           
//DEBUG 版本，怎么方便怎么配置，没有启动代码保护，内部E2PROM未启动，看门狗关闭,使用PLL*4，系统时钟为32MHz
__CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_ON & FCMEN_ON);
__CONFIG(WRT_OFF&PLLEN_OFF&LVP_OFF&STVREN_OFF&BORV_19&LVP_ON) ;//重新检查配置字 LVP_ON OFF重复
/* #elif       PIC_RELEASE_VERVISON
//释放版本，一定要启动代码保护，内部E2PROM数据保护
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_ON & BOREN_ON & CLKOUTEN_OFF & IESO_ON & FCMEN_ON);
__CONFIG(WRT_OFF&PLLEN_ON&LVP_OFF&STVREN_OFF&BORV_19&LVP_ON) ;
#endif*/
//更改MCU为低压版本 
#define VERSION_STR     "V1.1"
/*
 * 先初始化时钟，然后初始化外设（定时器，IO，ADC），PIC子函数调用不能超过8级
 */

/*
 * 系统时钟为内部RC时钟经过倍频得到，运行时钟为32MHz
 * 定时器为1ms进入一次
 */

//上电延时 power-up delay
//断电后，系统重新上电，断路器不响应控制信号保持初始状态的时间。大于4s
//自动合闸时间（tc）
//tc≤3s。
uint8_t flag_uart = 0;
//uint8_t Uart_Data[40];
extern uint8_t Ctr_Signal;         //是否收到控制信号
extern uint8_t Ctr_SignalBck;     //控制信号的备份值
//ZC低电平开关电源正常工作，高电平进入低功耗

enum
{
    NormalMode = 0,
    LowPowerMode = 1,
}RunMode_t;//运行模式
uint8_t isLowPower = NormalMode;//1为低功耗模式，0位正常运行模式
extern uint16_t numOf120ms;
int main(int argc, char** argv) 
{
 //   volatile unsigned long index = 0;
 //   uint8_t dat = 0x55;
 //   index++;
    uint8_t tmpbuf = 0;
    
    PIC_ConfigClock();//系统时钟1MHz
 
    PIC_ConfigOptionReg();
    PIC_ConfigGPIO();
    
    PIC_CofnigTimer0();
    PIC_EnableInterrupt();   //考虑是否需要向后移动
    KeyInit();              //按键初始化，负责控制手动模式和自动模式
    PowerSwitch_Init();              //低功耗管脚   
    Motor_Init();           //初始化电机管脚
    ADC_Init();             //控制引脚
    
    //PIC_ConfigUart();
   // DBG_UART_SENDSTR((uint8_t*)"start\r\n");
    
    internal_E2promInit();
    
#if 1      
    MOTOR_INA = 1;
    MOTOR_INB = 1;
    
    for(int i=0;i<6;i++)//上电延时4s    协议要求
    {
    //    MOTOR_INA = 1;
    //    MOTOR_INB = 0;
        //Delay_MS(500);
    //    MOTOR_INA = 0;
    //    MOTOR_INB = 1;
        //Delay_MS(500);
        //根据实际测试 缩短上电延时时间控制在9S左右
        Delay_MS(1200);//增加上电延时时间sxl
    }
#endif 
    if(Ctr_Signal == Ctrl_SignalValid)//一上电控制信号就有效
    {
#if 1
        if((gE2promState.systemState ==eE2State_AutoOpen ) || (gE2promState.systemState ==eE2State_AutoOpen ))//断电前是分闸状态，使用e2会出现一个问题，刚烧写完，可能会少一次动作
        {
             Ctr_SignalBck = Ctrl_SignalInvalid;//产生一次合闸信号
        }
        else
#endif
        Ctr_SignalBck = Ctr_Signal;         //电机不动作
       // DBG_UART_SENDSTR((uint8_t*)"no act\r\n");
    }
    else                              //控制信号无效，需要分闸，Ctr_Signal = Ctrl_SignalInvalid
    {
        Ctr_SignalBck = Ctrl_SignalValid;//产生一次分闸动作
        //DBG_UART_SENDSTR((uint8_t*)"act\r\n");
    }
    Motor_Stop();
    
    WatchDog_Init();      //看门狗
    isLowPower = NormalMode;//默认第一次上电不进入低功耗
    numOf120ms = 0;
    //SLEEP();
    while(1)
    {
        if(isLowPower == NormalMode)    //正常运行模式
        {
             Feed_WatchDog();
             KeyDriver();
             MainLoop();
            if(RA2 == 0)//在低功耗情况下断电了，立即退出低功耗
            {
               WatchDog_Reset();
            }
        }
        else if(isLowPower == LowPowerMode) //低功耗模式
        {
            INTCON &= ~(0x01 << 7);//禁止全局中断
            ADCON0  &= ~0x01;//失能ADC
            Motor_LowPower();
            PIC_ConfigClock32Khz();//降低时钟
            if(Ctr_Signal == Ctrl_SignalValid)//控制信号有效
            {
                tmpbuf = 0xff;
            }
            else //控制信号无效
            {
                tmpbuf = 0x00;
            }
            while(1)
            {
                if(RC1 == 0)
                {
                    tmpbuf = (tmpbuf << 1) | 0;
                }
                else if(RC1 == 1)
                {
                    tmpbuf = (tmpbuf << 1) | 1;
                }
                if((tmpbuf==0xff) && Ctr_Signal == Ctrl_SignalInvalid)//引脚从无信号，变成有信号，且控制信号状态为无效
                {
                    break;//退出低功耗
                }
                
                if((tmpbuf==0x00) && Ctr_Signal == Ctrl_SignalValid)//引脚从无信号，变成有信号，且控制信号状态为无效
                {
                    break;//退出低功耗
                }
                if(RA2 == 0)//在低功耗情况下断电了，立即退出低功耗
               {
                 WatchDog_Reset();
               }
                Feed_WatchDog();            
            }
            isLowPower = NormalMode;//切换到正常运行
            numOf120ms = 0;
            PIC_ConfigClock();//恢复系统时钟
            ADCON0  |= 0x01;//打开ADC
            PIC_EnableInterrupt();//打开所有中断
        }
    }
    return (EXIT_SUCCESS);
}
