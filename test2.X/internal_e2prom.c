#include "internal_e2prom.h"
#include "pic.h"
#include "config.h"
//��Ƭ���ڲ�e2prom



e2State_t gE2promState = {0};
//���һ�ϵ磬�ϴ��ϵ�ǰ��״̬�͵�ǰ��һ�¡�
//��ʼ����Ƭ���ڲ�e2prom
//uint8_t e2buf[32] = {0};
void internal_E2promInit(void)
{
    uint8_t addr = 0;
    EECON1 = 0; //Ĭ������Ϊ����E2Prom
    //internal_E2Write(addr,e2buf,32);
#if 1
   internal_E2Read(0,(uint8_t *)&gE2promState,sizeof(gE2promState));
   if(gE2promState.version  != 0xab)//�鿴�ǲ��ǳ�ʼ����
   {
       gE2promState.systemState = eE2State_Default; //ϵͳ��һ���ϵ�Ĭ��״̬
       gE2promState.version = 0xab;
       internal_E2Write(0,(uint8_t *)&gE2promState,sizeof(gE2promState));//��ʼ����Ĭ��ֵ
   }
#endif
#if 0
   internal_E2Read(addr,e2buf,32);//��ȡ�ڲ�e2prom ����
   
   Uart_SendDataLen(e2buf,32);
   
   for(int i=0;i<32;i++)
       e2buf[i]++;
   
   internal_E2Write(addr,e2buf,32);
#endif
}

//�ڲ�e2������
void internal_E2Read(uint8_t addr,uint8_t *dat,uint8_t len)
{
    while(len --)
    {
        while(RD == 1);
        //EEADRH = 0;
        EEADRL = addr++;
        EEPGD = 0;
        CFGS = 0;
        RD = 1;
        while(RD==1);
        //EECON1 |= (0x01 << 0);//��������־λ��RD��Ӳ������
        //NOP();NOP();
        *dat++ = EEDATL;//�����Ĵ�������
    }

}
//�ڲ�e2д����
void internal_E2Write(uint8_t addr,uint8_t *dat,uint8_t len)
{
    uint8_t reg = 0;
    INTCON &= ~(0x01 << 7);//��ֹȫ���ж�
    while(len --)
    {
        while(WR == 1);
        //EEADRH = 0;
        EEADRL = addr++;
        EEDATL = *dat++;
        //EEDATH = ;
        WREN = 1;
         EECON2 = 0x55;
         EECON2 = 0xAA;
         //EECON1  |= (0x01 << 1);//����WRλ
         WR = 1;
         while(WR==1);
         WREN = 0;
         //while(PIR2 & (0x01 << 4));//�ȴ�E2PROMд�������
         //PIR2 &= ~(0x01 << 4);//�����ɱ�־����ʼ��д��һλ
        
    }
    INTCON |= (0x01 << 7);//ʹ��ȫ���ж�
    
}
//����Ĭ������
void e2SavePara(e2Enum_t val)
{
    gE2promState.systemState = val;
    internal_E2Write(0,(uint8_t *)&gE2promState,sizeof(gE2promState));//��ʼ����Ĭ��ֵ
}
