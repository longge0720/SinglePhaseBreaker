#include "keyboard.h"
#include "config.h"
unsigned char KeySta = 1;//按键状态，默认按键松开
unsigned char AutoOrManualMode = AutoMode;//0:手动模式 1：自动模式
//按键初始化函数，初始化引脚为上拉
//硬件相关
//RA5
void KeyInit(void)
{
    TRISA   |= 0x01 << 5;
    PORTA   |= 0x01 << 5;//锁存1
    ANSELA  &= ~(0x01 << 5);//数字IO
    WPUA    |= 0x01 << 5;//使能弱上拉
}

void KeyDriver(void)
{
 	static unsigned char backup = 1;//弹起
    
    if(backup != KeySta)//状态改变了
    {
        if(KeySta == 0x00)//检测按键按下
        {
            KeyAction(0x00);//按键按下动作函数
        }
        else
        {
            KeyAction(0x01);//按键弹起动作函数
        }
        backup = KeySta;
    }
}

//定时器中断中进行扫描
void KeyScan(void)
{
 	static unsigned char KeyBuf = 0xff;

    KeyBuf = (KeyBuf << 1) | KEY;

	if(KeyBuf == 0xff)
    {
        KeySta = 0x01;
    }
    else if(KeyBuf == 0x00)
    {
        KeySta = 0x00;
    }
	
}

void KeyAction(unsigned char KeyCode)
{
    if(KeyCode == 0x00)//按键按下，人工模式
    {
        AutoOrManualMode = ManualMode;
    }
    else if(KeyCode == 0x01)//按键弹起自动模式
    {
        AutoOrManualMode = AutoMode;
    }
#if 0
       if(MOTOR_INA  == 1)
        {
            MOTOR_INA = 0;
        }
        else
        {
            MOTOR_INA = 1;
        }
#endif
}
