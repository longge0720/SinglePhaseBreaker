#include "motor.h"
#include "delay.h"
#include "lowpower.h"

#define USE_PIN_CONTROL_LOWPOWER                //使用引脚控制低功耗

uint8_t Motor_RunSta = 0;       //电机运行状态
uint16_t Motor_RunTime = 0;//电机运行时间

//电机引脚初始化
//RC5       INA
//RC2       INB
void Motor_Init(void)
{
    //初始化电机引脚
#if 1
    PORTC &= ~((0x01 << 2) | (0x01 << 5));//电机控制引脚清零 电机两个引脚默认输出低电平
    TRISC &= ~((0x01 << 2) | (0x01 << 5));//RC2 RC5 设置为输出
    ANSELC &= ~(0x01 << 2);
    ANSELC &= ~(0x01 << 5);
    WPUC &= ~(0x01 << 2);
    WPUC &= ~(0x01 << 5);
#endif
    //初始化霍尔引脚 RA4
    TRISA   |= 0x01 << 4;
    PORTA   |= 0x01 << 4;//锁存1
    ANSELA  &= ~(0x01 << 4);//数字IO
    WPUA    |= 0x01 << 4;//霍尔引脚使能弱上拉
}
//逆时针旋转
void Motor_Counterclockwise(void)//更改这里的两个变量
{
#ifdef USE_PIN_CONTROL_LOWPOWER
    //PowerSwitch_Enable();
   // Delay_MS(200);
#endif
#if 1
//    MOTOR_INA = 1;
//    MOTOR_INB = 0;//刹车
    uint8_t tem;
    tem = PORTC;
    tem &= ~(0x01 << 2);
    tem |= (0x01 << 5);
    PORTC = tem;
#endif
}
//顺时针Motor_Clockwise
void Motor_Clockwise(void)
{
    
#ifdef USE_PIN_CONTROL_LOWPOWER
    //PowerSwitch_Enable();
   // Delay_MS(200);
#endif
    

//    MOTOR_INA = 0;
//    MOTOR_INB = 1;
#if 1
    uint8_t tem;
    tem = PORTC;
    tem |= (0x01 << 2);
    tem &= ~(0x01 << 5);
    PORTC = tem;
#endif
}
//电机急停
void Motor_Stop(void)
{
//    MOTOR_INA = 1;
//    MOTOR_INB = 1;
    uint8_t tem;
    tem = PORTC;
    tem |= (0x01 << 2);
    tem |= (0x01 << 5);
    PORTC = tem;
    Motor_RunTime = 0;//电机停止时，电机运行时间清0
    
#ifdef USE_PIN_CONTROL_LOWPOWER
    Delay_MS(200);
    PowerSwitch_Disable();
#endif
}
//两个引脚同时为低电平，用于进入低功耗
void Motor_LowPower(void)
{
    uint8_t tem;
    tem = PORTC;
    tem &= ~(0x01 << 2);
    tem &= ~(0x01 << 5);
    PORTC = tem;
}

//获取电机状态信息
//低电平是获取到信号
int8_t Motor_GetStaus(void)
{
    if(HALL == 0)
    {
        return System_Closed;       //合闸状态
    }
    return System_Opened;
}
//电机运行时间,在中断中调用，中断时间为1ms
void Motor_RunTimeCount(void)
{
    //电机处于运行时才开始记录电机运行时间
    if((Motor_RunSta == Motor_AutoClosing) ||(Motor_RunSta ==  Motor_ManualClosing)||(Motor_RunSta == Motor_Opening) )//处于这三种状态下开始记录电机运行时间
    {
        Motor_RunTime++;//记录电机运行时间
    }
}