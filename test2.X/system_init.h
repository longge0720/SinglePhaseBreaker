/* 
 * File:   system_init.h
 * Author: USER
 *
 * Created on 2019��3��14��, ����10:03
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
    
void PIC_ConfigClock(void);//ϵͳʱ������
void PIC_ConfigClock32Khz(void);
void PIC_ConfigOptionReg(void);
void PIC_ConfigUartGPIO(void);
void PIC_CofnigTimer0(void);
void System_Init(void);

void WatchDog_Init(void);   //���Ź���ʼ������ʼ�����Ź���λʱ��
void Feed_WatchDog(void);    //ι�������ÿ��Ź���ʱ��
void WatchDog_Reset(void);
void PIC_ConfigGPIO(void);
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_INIT_H */

