/* 
 * File:   define.h
 * Author: USER
 *
 * Created on 2019年4月10日, 上午8:30
 */

#ifndef _DEFINE_H
#define	_DEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#define VDD_VAL         5   //系统供电电压
#define PIN_DEBUG           //使用引脚翻转调试
#define USE_UART_DEBUG              
//#define RELASE            //由于串口和电机控制使用了一个引脚，USE_UART_DEBUG 和 RELASE 只能定义一个，后期可以考虑串口复用
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

