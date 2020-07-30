/* 
 * File:   main.c
 * Author: USER
 *
 * Created on 2019��3��13��, ����3:16
 */

#include "define.h"
#include "config.h"
#include "uart.h"
#include <stdio.h>
#include "motor.h"
#include "internal_e2prom.h"
#include <htc.h>


//__CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_ON & FCMEN_ON);
//__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & BORV_19 & LVP_ON);
/*�����ź�Ϊ0.5V ~ 1.5V �ź� V1.0�汾*/
//#ifdef      PIC_DEBUG_VERSION           
//DEBUG �汾����ô������ô���ã�û���������뱣�����ڲ�E2PROMδ���������Ź��ر�,ʹ��PLL*4��ϵͳʱ��Ϊ32MHz
__CONFIG(FOSC_INTOSC & WDTE_SWDTEN & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_ON & FCMEN_ON);
__CONFIG(WRT_OFF&PLLEN_OFF&LVP_OFF&STVREN_OFF&BORV_19&LVP_ON) ;//���¼�������� LVP_ON OFF�ظ�
/* #elif       PIC_RELEASE_VERVISON
//�ͷŰ汾��һ��Ҫ�������뱣�����ڲ�E2PROM���ݱ���
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_ON & BOREN_ON & CLKOUTEN_OFF & IESO_ON & FCMEN_ON);
__CONFIG(WRT_OFF&PLLEN_ON&LVP_OFF&STVREN_OFF&BORV_19&LVP_ON) ;
#endif*/
//����MCUΪ��ѹ�汾 
#define VERSION_STR     "V1.1"
/*
 * �ȳ�ʼ��ʱ�ӣ�Ȼ���ʼ�����裨��ʱ����IO��ADC����PIC�Ӻ������ò��ܳ���8��
 */

/*
 * ϵͳʱ��Ϊ�ڲ�RCʱ�Ӿ�����Ƶ�õ�������ʱ��Ϊ32MHz
 * ��ʱ��Ϊ1ms����һ��
 */

//�ϵ���ʱ power-up delay
//�ϵ��ϵͳ�����ϵ磬��·������Ӧ�����źű��ֳ�ʼ״̬��ʱ�䡣����4s
//�Զ���բʱ�䣨tc��
//tc��3s��
uint8_t flag_uart = 0;
//uint8_t Uart_Data[40];
extern uint8_t Ctr_Signal;         //�Ƿ��յ������ź�
extern uint8_t Ctr_SignalBck;     //�����źŵı���ֵ
//ZC�͵�ƽ���ص�Դ�����������ߵ�ƽ����͹���

enum
{
    NormalMode = 0,
    LowPowerMode = 1,
}RunMode_t;//����ģʽ
uint8_t isLowPower = NormalMode;//1Ϊ�͹���ģʽ��0λ��������ģʽ
extern uint16_t numOf120ms;
int main(int argc, char** argv) 
{
 //   volatile unsigned long index = 0;
 //   uint8_t dat = 0x55;
 //   index++;
    uint8_t tmpbuf = 0;
    
    PIC_ConfigClock();//ϵͳʱ��1MHz
 
    PIC_ConfigOptionReg();
    PIC_ConfigGPIO();
    
    PIC_CofnigTimer0();
    PIC_EnableInterrupt();   //�����Ƿ���Ҫ����ƶ�
    KeyInit();              //������ʼ������������ֶ�ģʽ���Զ�ģʽ
    PowerSwitch_Init();              //�͹��Ĺܽ�   
    Motor_Init();           //��ʼ������ܽ�
    ADC_Init();             //��������
    
    //PIC_ConfigUart();
   // DBG_UART_SENDSTR((uint8_t*)"start\r\n");
    
    internal_E2promInit();
    
#if 1      
    MOTOR_INA = 1;
    MOTOR_INB = 1;
    
    for(int i=0;i<6;i++)//�ϵ���ʱ4s    Э��Ҫ��
    {
    //    MOTOR_INA = 1;
    //    MOTOR_INB = 0;
        //Delay_MS(500);
    //    MOTOR_INA = 0;
    //    MOTOR_INB = 1;
        //Delay_MS(500);
        //����ʵ�ʲ��� �����ϵ���ʱʱ�������9S����
        Delay_MS(1200);//�����ϵ���ʱʱ��sxl
    }
#endif 
    if(Ctr_Signal == Ctrl_SignalValid)//һ�ϵ�����źž���Ч
    {
#if 1
        if((gE2promState.systemState ==eE2State_AutoOpen ) || (gE2promState.systemState ==eE2State_AutoOpen ))//�ϵ�ǰ�Ƿ�բ״̬��ʹ��e2�����һ�����⣬����д�꣬���ܻ���һ�ζ���
        {
             Ctr_SignalBck = Ctrl_SignalInvalid;//����һ�κ�բ�ź�
        }
        else
#endif
        Ctr_SignalBck = Ctr_Signal;         //���������
       // DBG_UART_SENDSTR((uint8_t*)"no act\r\n");
    }
    else                              //�����ź���Ч����Ҫ��բ��Ctr_Signal = Ctrl_SignalInvalid
    {
        Ctr_SignalBck = Ctrl_SignalValid;//����һ�η�բ����
        //DBG_UART_SENDSTR((uint8_t*)"act\r\n");
    }
    Motor_Stop();
    
    WatchDog_Init();      //���Ź�
    isLowPower = NormalMode;//Ĭ�ϵ�һ���ϵ粻����͹���
    numOf120ms = 0;
    //SLEEP();
    while(1)
    {
        if(isLowPower == NormalMode)    //��������ģʽ
        {
             Feed_WatchDog();
             KeyDriver();
             MainLoop();
            if(RA2 == 0)//�ڵ͹�������¶ϵ��ˣ������˳��͹���
            {
               WatchDog_Reset();
            }
        }
        else if(isLowPower == LowPowerMode) //�͹���ģʽ
        {
            INTCON &= ~(0x01 << 7);//��ֹȫ���ж�
            ADCON0  &= ~0x01;//ʧ��ADC
            Motor_LowPower();
            PIC_ConfigClock32Khz();//����ʱ��
            if(Ctr_Signal == Ctrl_SignalValid)//�����ź���Ч
            {
                tmpbuf = 0xff;
            }
            else //�����ź���Ч
            {
                tmpbuf = 0x00;
            }
            while(1)
            {
                if(RC1 == 0)
                {
                    tmpbuf = (tmpbuf << 1) | 0;
                }
                else if(RC1 == 1)
                {
                    tmpbuf = (tmpbuf << 1) | 1;
                }
                if((tmpbuf==0xff) && Ctr_Signal == Ctrl_SignalInvalid)//���Ŵ����źţ�������źţ��ҿ����ź�״̬Ϊ��Ч
                {
                    break;//�˳��͹���
                }
                
                if((tmpbuf==0x00) && Ctr_Signal == Ctrl_SignalValid)//���Ŵ����źţ�������źţ��ҿ����ź�״̬Ϊ��Ч
                {
                    break;//�˳��͹���
                }
                if(RA2 == 0)//�ڵ͹�������¶ϵ��ˣ������˳��͹���
               {
                 WatchDog_Reset();
               }
                Feed_WatchDog();            
            }
            isLowPower = NormalMode;//�л�����������
            numOf120ms = 0;
            PIC_ConfigClock();//�ָ�ϵͳʱ��
            ADCON0  |= 0x01;//��ADC
            PIC_EnableInterrupt();//�������ж�
        }
    }
    return (EXIT_SUCCESS);
}
