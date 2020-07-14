/* 
 * File:   logic_control.h
 * Author: USER
 *
 * Created on 2019��4��10��, ����2:17
 */

#ifndef _LOGIC_CONTROL_H
#define	_LOGIC_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
    
typedef enum
{
    System_Opened = 0,   //��բ
    System_Closed,  //��բ
    
} SystemState_Typedef;

typedef enum
{
    Ctrl_SignalInvalid = 0,//�޿����ź�
    Ctrl_SignalValid, //�п����ź�   
}CtrlState_Typedef;//����״̬


void ContrlSignal_Driver(void);//�����ź������ж�
void Motor_Driver(void);
void MainLoop(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LOGIC_CONTROL_H */

