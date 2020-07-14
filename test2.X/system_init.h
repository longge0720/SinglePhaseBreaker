/* 
 * File:   system_init.h
 * Author: USER
 *
 * Created on 2019年3月14日, 上午10:03
 */

#ifndef SYSTEM_INIT_H
#define	SYSTEM_INIT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "pic.h"
#include <htc.h>
    
void PIC_ConfigClock(void);//系统时钟设置
void PIC_ConfigClock32Khz(void);
void PIC_ConfigOptionReg(void);
void PIC_ConfigUartGPIO(void);
void PIC_CofnigTimer0(void);
void System_Init(void);

void WatchDog_Init(void);   //看门狗初始化，初始化看门狗复位时间
void Feed_WatchDog(void);    //喂狗，重置看门狗定时器
void WatchDog_Reset(void);
void PIC_ConfigGPIO(void);
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_INIT_H */

