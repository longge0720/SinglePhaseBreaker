/* 
 * File:   motor.h
 * Author: USER
 *
 * Created on 2019年4月10日, 上午8:20
 */

#ifndef MOTOR_H
#define	MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
    
#define HALL_A                  RA2         //三相霍尔定义
#define HALL_B                  RA4
#define HALL_C                  RC5    
 
#define MOTOR_INA   RC2             //电机A相通道
#define MOTOR_INB   RC5             //电机B相通道
    
#define MOTOR_INA_H   (PORTC |= (0x01 << 2))             //电机A相通道
#define MOTOR_INA_L   (PORTC &= ~(0x01 << 2))             //电机A相通道

#define MOTOR_INB_H   (PORTC |= (0x01 << 4))              //电机B相通道
#define MOTOR_INB_L   (PORTC &= ~(0x01 << 4))              //电机B相通道
    
#define HALL        RA4
typedef enum 
{
    Motor_Default = 0,
    Motor_AutoClosing,//自动合闸
    Motor_ManualClosing,//手动合闸
    Motor_Opening,      //分闸
    Motor_Stoped,
    Motor_Err,
}MotorRunSta_Typedef;    

extern uint16_t Motor_RunTime;//电机运行时间  

void Motor_Init(void);
void Motor_Clockwise(void);
void Motor_Counterclockwise(void);
void Motor_Stop(void);
void Motor_LowPower(void);
void Motor_RunTimeCount(void);
int8_t Motor_GetStaus(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_H */

