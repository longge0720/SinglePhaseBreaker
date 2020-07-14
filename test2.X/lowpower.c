#include "lowpower.h"

#define POWER_SWITCH_READY_TIME     300

static uint16_t PowerSwitch_Cnt = 0;//��Դ����ʱ�䣬�ȴ����ص�Դ����
static uint8_t PowerSwitch_EnableCnt = 0;//ʹ�ܿ��ص�Դ����

//�͵�ƽ�����������ߵ�ƽ������
void PowerSwitch_Init(void)              //���ڿ����Ƿ�ʵ�ֵ͹��Ĺ���
{
    TRISC   &= ~(0x01 << 3);
    PORTC   &= ~( 0x01 << 3);//����0
    ANSELC  &= ~(0x01 << 3);//����IO
}
static void PowerSwitch_Delay(void)
{
     uint8_t delay = 0;
    for(delay=0;delay<20;delay++);
}
//���ص�Դ��ʼ����
void PowerSwitch_Enable(void)
{
    uint8_t tmp = 0;
    PowerSwitch_Delay();
    tmp = PORTC;
    tmp &= ~(0x01 << 3);
    PORTC = tmp;
}
//���ص�Դֹͣ����
void PowerSwitch_Disable(void)
{
    uint8_t tmp = 0;
    
    PowerSwitch_Delay();
    tmp = PORTC;
    tmp |= (0x01 << 3);
    PORTC = tmp;

}

// 1:���ص�Դ�Ѿ�����
// -1 ���ص�Դδ����
//����Ҫ�����͹���ʱ����
int PowerSwitch_WaitReady(void)
{
    int ret = 0;
    PowerSwitch_EnableCnt = 1;//���ص�Դ��ʼ����
    PowerSwitch_Enable();//ʹ�ܿ��ص�Դ����
    if(PowerSwitch_Cnt > POWER_SWITCH_READY_TIME)//�����Դ����ʱ�䵽����ô����1��������ص�Դ����ʱ��
    {
        PowerSwitch_EnableCnt = 0;          //�رտ��ص�Դ����
        PowerSwitch_Cnt = 0;
        ret = 1;        
    }
    else
    {
        ret = -1;
    }
    return ret;
}

//�ж�������
void PowerSwitch_Scan(void)
{
    if(PowerSwitch_EnableCnt)
    {
        PowerSwitch_Cnt++;
    }
}
