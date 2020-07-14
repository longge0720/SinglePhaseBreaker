#include "signal.h"
#include "adc.h"
#include "uart.h"
#include "config.h"

#define SIGNAL_PIN              RC1
#define ADC_THRESHOLD           500 //((uint16_t)(0.1*1024/VDD_VAL))      //使用ADC的阈值为0.4V
#define SIGNAL_EFFECIVE         0          //控制信号是高电平有效还是低电平有效
#define SIGNAL_JUDGE_VAL         500         //信号个数判断，超过判定值为有效
#define SIGNAL_PERIOD           1000        //信号周期

#define MOTOR_TIMEOUT           6000        //6s

extern uint8_t Ctr_Signal;         //是否收到控制信号
extern uint8_t Ctr_SignalBck;     //控制信号的备份值
extern uint8_t Motor_RunSta;       //电机运行状态

extern uint8_t isLowPower;          //是否进入低功耗模式

uint16_t gPhaseVoltage_CH1 = 0;//相线电压,值比较大的
uint16_t gPhaseVoltage_CH2 = 0;//值比较小的
#if 0
extern uint8_t Uart_Data[40];
extern uint8_t flag_uart;
uint8_t int_div = 0;
uint8_t Uart_DataNum = 0;
#endif
uint32_t average = 0;
uint16_t numOf120ms = 0;//运行了多少个120ms
//中断中查询是否收到控制信号
//在一个周期内判断有效值个数，一旦有效值个数大于SIGNAL_JUDGE_VAL就认为收到相应控制信号了
void Contrl_Signal(void)
{
    static uint16_t period = 0;//50hz周期
    static uint16_t signal_cnt = 0;//信号计数值 1s周期内，有多少个高电平
    static uint8_t ctrBuf = 0;
    uint16_t tem;
    
    
    period++;   //测量周期
    //tem = Get_ADCValue(eADC_AN5);//通道5是检测控制信号
    
    //if(tem > ADC_THRESHOLD)
    if(RC1 == 1)//RC1是控制信号
    {
        signal_cnt++;       //控制信号有效
    }
    if(period >= 80)//到达一个周期时间,120ms为1个周期
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
        //显示其他通道ADC电压值
        //DBG_UART_SENDSTR((uint8_t*)"fire ");
        gPhaseVoltage_CH1 = Get_ADCValue(eADC_AN4);   //电压比较低的相线电压 RA1->AN1
        //DBG_SHOW_INT(gPhaseVoltage_l);
        
        /*gPhaseVoltage_CH2 = Get_ADCValue(eADC_AN2);  */   //电压比较高的相线电压,RA2->AN2
        
        //DBG_SHOW_INT(gPhaseVoltage_h);
        //结束显示其他通道电压值
        //DBG_UART_SENDSTR((uint8_t*)"auto=0 ");
        //DBG_SHOW_INT(AutoOrManualMode);
        
        if(signal_cnt > 50)
        {
            ctrBuf = (ctrBuf << 1) |0x01;
            // Ctr_Signal = Ctrl_SignalValid;//控制信号有效
             //DBG_UART_SENDSTR((uint8_t*)"sig\r\n");
        }
        else
        {
            ctrBuf = (ctrBuf << 1) |0x00;
            //Ctr_Signal = Ctrl_SignalInvalid;//控制信号无效
            //DBG_UART_SENDSTR((uint8_t*)"no sig\r\n");
        }
        
        if(ctrBuf == 0xff)
        {
            Ctr_Signal = Ctrl_SignalValid;//控制信号有效
        }
        else if(ctrBuf == 0x00)
        {
            Ctr_Signal = Ctrl_SignalInvalid;//控制信号无效
        }
        
        if((numOf120ms > 100) && (Ctr_Signal== Ctr_SignalBck) &&(Motor_RunSta==Motor_Stoped ||Motor_RunSta== Motor_Default) &&(RA2==1)) // && (RC0 == 1)//进入低功耗条件，控制信号没有变化，电机停止动作，火线有电压
        {
            numOf120ms = 0;
            isLowPower = 1;
        }
        average = 0;
        period = 0;
        signal_cnt = 0;
    }

}
