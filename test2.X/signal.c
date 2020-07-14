#include "signal.h"
#include "adc.h"
#include "uart.h"
#include "config.h"

#define SIGNAL_PIN              RC1
#define ADC_THRESHOLD           500 //((uint16_t)(0.1*1024/VDD_VAL))      //ʹ��ADC����ֵΪ0.4V
#define SIGNAL_EFFECIVE         0          //�����ź��Ǹߵ�ƽ��Ч���ǵ͵�ƽ��Ч
#define SIGNAL_JUDGE_VAL         500         //�źŸ����жϣ������ж�ֵΪ��Ч
#define SIGNAL_PERIOD           1000        //�ź�����

#define MOTOR_TIMEOUT           6000        //6s

extern uint8_t Ctr_Signal;         //�Ƿ��յ������ź�
extern uint8_t Ctr_SignalBck;     //�����źŵı���ֵ
extern uint8_t Motor_RunSta;       //�������״̬

extern uint8_t isLowPower;          //�Ƿ����͹���ģʽ

uint16_t gPhaseVoltage_CH1 = 0;//���ߵ�ѹ,ֵ�Ƚϴ��
uint16_t gPhaseVoltage_CH2 = 0;//ֵ�Ƚ�С��
#if 0
extern uint8_t Uart_Data[40];
extern uint8_t flag_uart;
uint8_t int_div = 0;
uint8_t Uart_DataNum = 0;
#endif
uint32_t average = 0;
uint16_t numOf120ms = 0;//�����˶��ٸ�120ms
//�ж��в�ѯ�Ƿ��յ������ź�
//��һ���������ж���Чֵ������һ����Чֵ��������SIGNAL_JUDGE_VAL����Ϊ�յ���Ӧ�����ź���
void Contrl_Signal(void)
{
    static uint16_t period = 0;//50hz����
    static uint16_t signal_cnt = 0;//�źż���ֵ 1s�����ڣ��ж��ٸ��ߵ�ƽ
    static uint8_t ctrBuf = 0;
    uint16_t tem;
    
    
    period++;   //��������
    //tem = Get_ADCValue(eADC_AN5);//ͨ��5�Ǽ������ź�
    
    //if(tem > ADC_THRESHOLD)
    if(RC1 == 1)//RC1�ǿ����ź�
    {
        signal_cnt++;       //�����ź���Ч
    }
    if(period >= 80)//����һ������ʱ��,120msΪ1������
    {
     //   average /= 1000;
    //    Uart_SendDataLen(&average,1);
        //DBG_UART_SENDSTR((uint8_t*)"cnt ");
        //DBG_SHOW_INT(signal_cnt);
        
        //DBG_UART_SENDSTR((uint8_t*)"hal ");
        //DBG_SHOW_INT(HALL);
        
        //DBG_UART_SENDSTR((uint8_t*)"adc ");
        uint16_t dis_tmp = 0;
        numOf120ms++;
        //dis_tmp = Get_ADCValue(eADC_AN5);
        //DBG_SHOW_INT(dis_tmp);
        //��ʾ����ͨ��ADC��ѹֵ
        //DBG_UART_SENDSTR((uint8_t*)"fire ");
        gPhaseVoltage_CH1 = Get_ADCValue(eADC_AN4);   //��ѹ�Ƚϵ͵����ߵ�ѹ RA1->AN1
        //DBG_SHOW_INT(gPhaseVoltage_l);
        
        /*gPhaseVoltage_CH2 = Get_ADCValue(eADC_AN2);  */   //��ѹ�Ƚϸߵ����ߵ�ѹ,RA2->AN2
        
        //DBG_SHOW_INT(gPhaseVoltage_h);
        //������ʾ����ͨ����ѹֵ
        //DBG_UART_SENDSTR((uint8_t*)"auto=0 ");
        //DBG_SHOW_INT(AutoOrManualMode);
        
        if(signal_cnt > 50)
        {
            ctrBuf = (ctrBuf << 1) |0x01;
            // Ctr_Signal = Ctrl_SignalValid;//�����ź���Ч
             //DBG_UART_SENDSTR((uint8_t*)"sig\r\n");
        }
        else
        {
            ctrBuf = (ctrBuf << 1) |0x00;
            //Ctr_Signal = Ctrl_SignalInvalid;//�����ź���Ч
            //DBG_UART_SENDSTR((uint8_t*)"no sig\r\n");
        }
        
        if(ctrBuf == 0xff)
        {
            Ctr_Signal = Ctrl_SignalValid;//�����ź���Ч
        }
        else if(ctrBuf == 0x00)
        {
            Ctr_Signal = Ctrl_SignalInvalid;//�����ź���Ч
        }
        
        if((numOf120ms > 100) && (Ctr_Signal== Ctr_SignalBck) &&(Motor_RunSta==Motor_Stoped ||Motor_RunSta== Motor_Default) &&(RA2==1)) // && (RC0 == 1)//����͹��������������ź�û�б仯�����ֹͣ�����������е�ѹ
        {
            numOf120ms = 0;
            isLowPower = 1;
        }
        average = 0;
        period = 0;
        signal_cnt = 0;
    }

}
