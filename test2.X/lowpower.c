#include "lowpower.h"

#define POWER_SWITCH_READY_TIME     300

static uint16_t PowerSwitch_Cnt = 0;//电源运行时间，等待开关电源就绪
static uint8_t PowerSwitch_EnableCnt = 0;//使能开关电源计数

//低电平正常工作，高电平不工作
void PowerSwitch_Init(void)              //后期考虑是否实现低功耗功能
{
    TRISC   &= ~(0x01 << 3);
    PORTC   &= ~( 0x01 << 3);//锁存0
    ANSELC  &= ~(0x01 << 3);//数字IO
}
static void PowerSwitch_Delay(void)
{
     uint8_t delay = 0;
    for(delay=0;delay<20;delay++);
}
//开关电源开始工作
void PowerSwitch_Enable(void)
{
    uint8_t tmp = 0;
    PowerSwitch_Delay();
    tmp = PORTC;
    tmp &= ~(0x01 << 3);
    PORTC = tmp;
}
//开关电源停止工作
void PowerSwitch_Disable(void)
{
    uint8_t tmp = 0;
    
    PowerSwitch_Delay();
    tmp = PORTC;
    tmp |= (0x01 << 3);
    PORTC = tmp;

}

// 1:开关电源已经启动
// -1 开关电源未启动
//在需要启动低功耗时调用
int PowerSwitch_WaitReady(void)
{
    int ret = 0;
    PowerSwitch_EnableCnt = 1;//开关电源开始计数
    PowerSwitch_Enable();//使能开关电源引脚
    if(PowerSwitch_Cnt > POWER_SWITCH_READY_TIME)//如果电源启动时间到，那么返回1，清除开关电源计数时间
    {
        PowerSwitch_EnableCnt = 0;          //关闭开关电源计数
        PowerSwitch_Cnt = 0;
        ret = 1;        
    }
    else
    {
        ret = -1;
    }
    return ret;
}

//中断中运行
void PowerSwitch_Scan(void)
{
    if(PowerSwitch_EnableCnt)
    {
        PowerSwitch_Cnt++;
    }
}
