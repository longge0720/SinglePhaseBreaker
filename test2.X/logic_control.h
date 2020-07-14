/* 
 * File:   logic_control.h
 * Author: USER
 *
 * Created on 2019年4月10日, 下午2:17
 */

#ifndef _LOGIC_CONTROL_H
#define	_LOGIC_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
    
typedef enum
{
    System_Opened = 0,   //分闸
    System_Closed,  //合闸
    
} SystemState_Typedef;

typedef enum
{
    Ctrl_SignalInvalid = 0,//无控制信号
    Ctrl_SignalValid, //有控制信号   
}CtrlState_Typedef;//控制状态


void ContrlSignal_Driver(void);//控制信号驱动判断
void Motor_Driver(void);
void MainLoop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LOGIC_CONTROL_H */

