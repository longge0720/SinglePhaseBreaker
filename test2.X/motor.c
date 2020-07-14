#include "motor.h"
#include "delay.h"
#include "lowpower.h"

#define USE_PIN_CONTROL_LOWPOWER                //ʹ�����ſ��Ƶ͹���

uint8_t Motor_RunSta = 0;       //�������״̬
uint16_t Motor_RunTime = 0;//�������ʱ��

//������ų�ʼ��
//RC5       INA
//RC2       INB
void Motor_Init(void)
{
    //��ʼ���������
#if 1
    PORTC &= ~((0x01 << 2) | (0x01 << 5));//��������������� �����������Ĭ������͵�ƽ
    TRISC &= ~((0x01 << 2) | (0x01 << 5));//RC2 RC5 ����Ϊ���
    ANSELC &= ~(0x01 << 2);
    ANSELC &= ~(0x01 << 5);
    WPUC &= ~(0x01 << 2);
    WPUC &= ~(0x01 << 5);
#endif
    //��ʼ���������� RA4
    TRISA   |= 0x01 << 4;
    PORTA   |= 0x01 << 4;//����1
    ANSELA  &= ~(0x01 << 4);//����IO
    WPUA    |= 0x01 << 4;//��������ʹ��������
}
//��ʱ����ת
void Motor_Counterclockwise(void)//�����������������
{
#ifdef USE_PIN_CONTROL_LOWPOWER
    //PowerSwitch_Enable();
   // Delay_MS(200);
#endif
#if 1
//    MOTOR_INA = 1;
//    MOTOR_INB = 0;//ɲ��
    uint8_t tem;
    tem = PORTC;
    tem &= ~(0x01 << 2);
    tem |= (0x01 << 5);
    PORTC = tem;
#endif
}
//˳ʱ��Motor_Clockwise
void Motor_Clockwise(void)
{
    
#ifdef USE_PIN_CONTROL_LOWPOWER
    //PowerSwitch_Enable();
   // Delay_MS(200);
#endif
    

//    MOTOR_INA = 0;
//    MOTOR_INB = 1;
#if 1
    uint8_t tem;
    tem = PORTC;
    tem |= (0x01 << 2);
    tem &= ~(0x01 << 5);
    PORTC = tem;
#endif
}
//�����ͣ
void Motor_Stop(void)
{
//    MOTOR_INA = 1;
//    MOTOR_INB = 1;
    uint8_t tem;
    tem = PORTC;
    tem |= (0x01 << 2);
    tem |= (0x01 << 5);
    PORTC = tem;
    Motor_RunTime = 0;//���ֹͣʱ���������ʱ����0
    
#ifdef USE_PIN_CONTROL_LOWPOWER
    Delay_MS(200);
    PowerSwitch_Disable();
#endif
}
//��������ͬʱΪ�͵�ƽ�����ڽ���͹���
void Motor_LowPower(void)
{
    uint8_t tem;
    tem = PORTC;
    tem &= ~(0x01 << 2);
    tem &= ~(0x01 << 5);
    PORTC = tem;
}

//��ȡ���״̬��Ϣ
//�͵�ƽ�ǻ�ȡ���ź�
int8_t Motor_GetStaus(void)
{
    if(HALL == 0)
    {
        return System_Closed;       //��բ״̬
    }
    return System_Opened;
}
//�������ʱ��,���ж��е��ã��ж�ʱ��Ϊ1ms
void Motor_RunTimeCount(void)
{
    //�����������ʱ�ſ�ʼ��¼�������ʱ��
    if((Motor_RunSta == Motor_AutoClosing) ||(Motor_RunSta ==  Motor_ManualClosing)||(Motor_RunSta == Motor_Opening) )//����������״̬�¿�ʼ��¼�������ʱ��
    {
        Motor_RunTime++;//��¼�������ʱ��
    }
}