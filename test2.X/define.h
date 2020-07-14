/* 
 * File:   define.h
 * Author: USER
 *
 * Created on 2019��4��10��, ����8:30
 */

#ifndef _DEFINE_H
#define	_DEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#define VDD_VAL         5   //ϵͳ�����ѹ
#define PIN_DEBUG           //ʹ�����ŷ�ת����
#define USE_UART_DEBUG              
//#define RELASE            //���ڴ��ں͵������ʹ����һ�����ţ�USE_UART_DEBUG �� RELASE ֻ�ܶ���һ�������ڿ��Կ��Ǵ��ڸ���
#define VERSION         1.1
    
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
    
#include <htc.h>
#include "pic.h"
    
#include "keyboard.h"
#include "interrupt.h"
#include "system_init.h"
#include "motor.h"   
#include "logic_control.h"
#include "delay.h"
#include "signal.h"
#include "adc.h"
#include "lowpower.h"
#ifdef	__cplusplus
}
#endif

#endif	/* DEFINE_H */

