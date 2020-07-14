/* 
 * File:   motor.h
 * Author: USER
 *
 * Created on 2019��4��10��, ����8:20
 */

#ifndef MOTOR_H
#define	MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
    
#define HALL_A                  RA2         //�����������
#define HALL_B                  RA4
#define HALL_C                  RC5    
 
#define MOTOR_INA   RC2             //���A��ͨ��
#define MOTOR_INB   RC5             //���B��ͨ��
    
#define MOTOR_INA_H   (PORTC |= (0x01 << 2))             //���A��ͨ��
#define MOTOR_INA_L   (PORTC &= ~(0x01 << 2))             //���A��ͨ��

#define MOTOR_INB_H   (PORTC |= (0x01 << 4))              //���B��ͨ��
#define MOTOR_INB_L   (PORTC &= ~(0x01 << 4))              //���B��ͨ��
    
#define HALL        RA4
typedef enum 
{
    Motor_Default = 0,
    Motor_AutoClosing,//�Զ���բ
    Motor_ManualClosing,//�ֶ���բ
    Motor_Opening,      //��բ
    Motor_Stoped,
    Motor_Err,
}MotorRunSta_Typedef;    

extern uint16_t Motor_RunTime;//�������ʱ��  

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

