/*
 *
 * 控制逻辑
 * 
 */
#include "uart.h"
#include"define.h"
#include "config.h"
#include "delay.h"
#include "internal_e2prom.h"
//自动模式逆时针旋转，合闸旋转到霍尔元器件位置上，分闸脱离霍尔元器件位置旋转大约1s
//手动模式顺时针旋转，控制信号有效时旋转到霍尔元器件位置上，无效时逆时针旋转1s
//只有手动模式时是顺时针旋转
//超时时间为6S左右，超过6s则不旋转
//分合闸状态切换手动自动开关？？？？？？？？？
//相线掉电缺电时状态

//逻辑层初始化
//控制信号

uint8_t Ctr_Signal;         //是否收到控制信号
uint8_t Ctr_SignalBck;     //控制信号的备份值

uint8_t System_Sta;         //系统状态，也就是门是否开合闸状态
uint8_t System_StaBck;      //系统状态备份值

extern uint8_t Motor_RunSta;       //电机运行状态

extern uint16_t gPhaseVoltage_CH1;//
extern uint16_t gPhaseVoltage_CH2;
//第一次上电就有控制信号，默认为控制信号一直存在

//是否开合闸是由 Ctr_Signal 判断
void ContrlSignal_Driver(void)//控制信号驱动判断
{

    //if(Ctr_SignalBck != Ctr_Signal)             //控制信号在变化时才有效，且电机处于停止，或者初始化    
    if((Ctr_SignalBck != Ctr_Signal) &&(Motor_RunSta==Motor_Stoped ||Motor_RunSta== Motor_Default) &&(gPhaseVoltage_CH1 > 200) )//电机控制信号判断有效的情况下，必须保证一次完整的行程。不能卡在一半就停止。且相线电压未掉电
    {         
        PowerSwitch_Enable();               //信号有变化时候打开 开关电源
        Delay_MS(200);                      
        
        System_Sta = Motor_GetStaus();//获取电机当前状态
        if(Ctr_Signal == Ctrl_SignalValid)//控制信号有效
        {
            if(System_Sta == System_Closed) //当前已经处于合闸状态
            {                
                Motor_Stop();
            }
            else if(System_Sta == System_Opened)//当前处于分闸状态
            {
                if(AutoOrManualMode == AutoMode)    //自动模式
                {
                    DBG_UART_SENDSTR((uint8_t*)"C auto close\r\n");
                    Motor_RunSta = Motor_AutoClosing;                           //电机信号生产者
                    Motor_Counterclockwise();//电机开始工作，逆时针旋转
                }
                else if(AutoOrManualMode == ManualMode)//手动模式
                {
                    DBG_UART_SENDSTR((uint8_t*)"C man close\r\n");
                    Motor_RunSta = Motor_ManualClosing;
                    Motor_Clockwise();//顺时针旋转
                }
                
            }
        }
        else if(Ctr_Signal == Ctrl_SignalInvalid) //分闸信号
        {
            
            if(System_Sta == System_Closed)
            {
                DBG_UART_SENDSTR((uint8_t*)"C open\r\n");
                Motor_RunSta = Motor_Opening;//分闸
                Motor_Counterclockwise();    //分闸信号电机大约运行1s
            }
            else if(System_Sta == System_Opened)
            {
                 DBG_UART_SENDSTR((uint8_t*)"C stop\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//电机停止运行
            }
        }
        Ctr_SignalBck = Ctr_Signal;
    }
}

//判断两个状态
//电机状态，当前是否合闸状态
void Motor_Driver(void)
{
    System_Sta = Motor_GetStaus();//获取电机当前状态
    //电机处于运行状态
    switch(Motor_RunSta)//电机运行状态
    {
        case Motor_AutoClosing://电机自动开关模式
            if((System_Sta == System_Closed)&&(Motor_RunTime < 6000))//已经合闸，且电机运行时间未超时
            {
                DBG_UART_SENDSTR((uint8_t*)"M auto close\r\n");
                Motor_RunSta = Motor_Stoped;                                    //电机信号使用者，且发送停止信号
                Delay_MS(75);//在这个地方增加延时
                Motor_Stop();//电机停止运行
                e2SavePara(eE2State_AutoClose);  
    
            }
            else if(Motor_RunTime >= 6000)//电机运行时间超时
            {
                DBG_UART_SENDSTR((uint8_t*)"M auto close timeout\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //错误处理函数
            }
            break;
        case Motor_ManualClosing:
            if((System_Sta == System_Closed)&&(Motor_RunTime < 6000))//已经合闸，且电机运行时间未超时
            {
                DBG_UART_SENDSTR((uint8_t*)"M man close\r\n");
                Delay_MS(2);
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//电机停止运行
                e2SavePara(eE2State_ManClose);
            }
            else if(Motor_RunTime >= 6000)//电机运行时间超时
            {
                DBG_UART_SENDSTR((uint8_t*)"M man close timeout\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //错误处理函数
            }
            break;
        case Motor_Opening:                                           //分闸
            if((System_Sta == System_Opened)&&(Motor_RunTime  > 500))//已经分闸，且运行大约800ms，之前是400ms
            {
                DBG_UART_SENDSTR((uint8_t*)"M open\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//电机停止运行
                e2SavePara(eE2State_AutoOpen);              //自动分闸手动分闸用一个函数
            }
            else if(Motor_RunTime >= 6000)//电机运行时间超时
            {
                DBG_UART_SENDSTR((uint8_t*)"M open timeout\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //错误处理函数
            }
            break;
            
        default:break;
    }
            
}

void MainLoop(void)
{
    ContrlSignal_Driver();
    Motor_Driver();
}

