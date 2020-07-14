/* 
 * File:   keyboard.h
 * Author: USER
 *
 * Created on 2019年4月10日, 上午9:32
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

extern uint8_t AutoOrManualMode;     //自动手动模式切换
extern unsigned char KeySta;//按键状态，默认按键松开

void KeyInit(void);
void KeyDriver(void);
void KeyScan(void);

extern void KeyAction(unsigned char KeyCode);
#endif	/* KEYBOARD_H */
