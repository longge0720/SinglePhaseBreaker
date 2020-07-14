/* 
 * File:   keyboard.h
 * Author: USER
 *
 * Created on 2019��4��10��, ����9:32
 */

#ifndef _KEYBOARD_H
#define	_KEYBOARD_H

#include "define.h"

#define KEY         RA5

typedef enum
{
    AutoMode = 0,
    ManualMode =1,      
}RunMode_Typedef;

extern uint8_t AutoOrManualMode;     //�Զ��ֶ�ģʽ�л�
extern unsigned char KeySta;//����״̬��Ĭ�ϰ����ɿ�

void KeyInit(void);
void KeyDriver(void);
void KeyScan(void);

extern void KeyAction(unsigned char KeyCode);
#endif	/* KEYBOARD_H */
