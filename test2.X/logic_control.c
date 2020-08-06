/*
 *
 * �����߼�
 * 
 */
#include "uart.h"
#include"define.h"
#include "config.h"
#include "delay.h"
#include "internal_e2prom.h"
//�Զ�ģʽ��ʱ����ת����բ��ת������Ԫ����λ���ϣ���բ�������Ԫ����λ����ת��Լ1s
//�ֶ�ģʽ˳ʱ����ת�������ź���Чʱ��ת������Ԫ����λ���ϣ���Чʱ��ʱ����ת1s
//ֻ���ֶ�ģʽʱ��˳ʱ����ת
//��ʱʱ��Ϊ6S���ң�����6s����ת
//�ֺ�բ״̬�л��ֶ��Զ����أ�����������������
//���ߵ���ȱ��ʱ״̬

//�߼����ʼ��
//�����ź�

uint8_t Ctr_Signal;         //�Ƿ��յ������ź�
uint8_t Ctr_SignalBck;     //�����źŵı���ֵ

uint8_t System_Sta;         //ϵͳ״̬��Ҳ�������Ƿ񿪺�բ״̬
uint8_t System_StaBck;      //ϵͳ״̬����ֵ

extern uint8_t Motor_RunSta;       //�������״̬

extern uint16_t gPhaseVoltage_CH1;//
extern uint16_t gPhaseVoltage_CH2;
//��һ���ϵ���п����źţ�Ĭ��Ϊ�����ź�һֱ����

//�Ƿ񿪺�բ���� Ctr_Signal �ж�
void ContrlSignal_Driver(void)//�����ź������ж�
{

    //if(Ctr_SignalBck != Ctr_Signal)             //�����ź��ڱ仯ʱ����Ч���ҵ������ֹͣ�����߳�ʼ��    
    if((Ctr_SignalBck != Ctr_Signal) &&(Motor_RunSta==Motor_Stoped ||Motor_RunSta== Motor_Default) &&(gPhaseVoltage_CH1 > 200) )//��������ź��ж���Ч������£����뱣֤һ���������г̡����ܿ���һ���ֹͣ�������ߵ�ѹδ����
    {         
        PowerSwitch_Enable();               //�ź��б仯ʱ��� ���ص�Դ
        Delay_MS(200);                      
        
        System_Sta = Motor_GetStaus();//��ȡ�����ǰ״̬
        if(Ctr_Signal == Ctrl_SignalValid)//�����ź���Ч
        {
            if(System_Sta == System_Closed) //��ǰ�Ѿ����ں�բ״̬
            {                
                Motor_Stop();
            }
            else if(System_Sta == System_Opened)//��ǰ���ڷ�բ״̬
            {
                if(AutoOrManualMode == AutoMode)    //�Զ�ģʽ
                {
                    DBG_UART_SENDSTR((uint8_t*)"C auto close\r\n");
                    Motor_RunSta = Motor_AutoClosing;                           //����ź�������
                    Motor_Counterclockwise();//�����ʼ��������ʱ����ת
                }
                else if(AutoOrManualMode == ManualMode)//�ֶ�ģʽ
                {
                    DBG_UART_SENDSTR((uint8_t*)"C man close\r\n");
                    Motor_RunSta = Motor_ManualClosing;
                    Motor_Clockwise();//˳ʱ����ת
                }
                
            }
        }
        else if(Ctr_Signal == Ctrl_SignalInvalid) //��բ�ź�
        {
            
            if(System_Sta == System_Closed)
            {
                DBG_UART_SENDSTR((uint8_t*)"C open\r\n");
                Motor_RunSta = Motor_Opening;//��բ
                Motor_Counterclockwise();    //��բ�źŵ����Լ����1s
            }
            else if(System_Sta == System_Opened)
            {
                 DBG_UART_SENDSTR((uint8_t*)"C stop\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//���ֹͣ����
            }
        }
        Ctr_SignalBck = Ctr_Signal;
    }
}

//�ж�����״̬
//���״̬����ǰ�Ƿ��բ״̬
void Motor_Driver(void)
{
    System_Sta = Motor_GetStaus();//��ȡ�����ǰ״̬
    //�����������״̬
    switch(Motor_RunSta)//�������״̬
    {
        case Motor_AutoClosing://����Զ�����ģʽ
            if((System_Sta == System_Closed)&&(Motor_RunTime < 6000))//�Ѿ���բ���ҵ������ʱ��δ��ʱ
            {
                DBG_UART_SENDSTR((uint8_t*)"M auto close\r\n");
                Motor_RunSta = Motor_Stoped;                                    //����ź�ʹ���ߣ��ҷ���ֹͣ�ź�
                Delay_MS(75);//������ط�������ʱ
                Motor_Stop();//���ֹͣ����
                e2SavePara(eE2State_AutoClose);  
    
            }
            else if(Motor_RunTime >= 6000)//�������ʱ�䳬ʱ
            {
                DBG_UART_SENDSTR((uint8_t*)"M auto close timeout\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //��������
            }
            break;
        case Motor_ManualClosing:
            if((System_Sta == System_Closed)&&(Motor_RunTime < 6000))//�Ѿ���բ���ҵ������ʱ��δ��ʱ
            {
                DBG_UART_SENDSTR((uint8_t*)"M man close\r\n");
                Delay_MS(2);
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//���ֹͣ����
                e2SavePara(eE2State_ManClose);
            }
            else if(Motor_RunTime >= 6000)//�������ʱ�䳬ʱ
            {
                DBG_UART_SENDSTR((uint8_t*)"M man close timeout\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //��������
            }
            break;
        case Motor_Opening:                                           //��բ
            if((System_Sta == System_Opened)&&(Motor_RunTime  > 500))//�Ѿ���բ�������д�Լ800ms��֮ǰ��400ms
            {
                DBG_UART_SENDSTR((uint8_t*)"M open\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();//���ֹͣ����
                e2SavePara(eE2State_AutoOpen);              //�Զ���բ�ֶ���բ��һ������
            }
            else if(Motor_RunTime >= 6000)//�������ʱ�䳬ʱ
            {
                DBG_UART_SENDSTR((uint8_t*)"M open timeout\r\n");
                Motor_RunSta = Motor_Stoped;
                Motor_Stop();
                //��������
            }
            break;
            
        default:break;
    }
            
}

void MainLoop(void)
{
    ContrlSignal_Driver();
    Motor_Driver();
}

