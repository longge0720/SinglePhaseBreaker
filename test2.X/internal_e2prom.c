#include "internal_e2prom.h"
#include "pic.h"
#include "config.h"
//单片机内部e2prom



e2State_t gE2promState = {0};
//如果一上电，上次上电前的状态和当前不一致。
//初始化单片机内部e2prom
//uint8_t e2buf[32] = {0};
void internal_E2promInit(void)
{
    uint8_t addr = 0;
    EECON1 = 0; //默认设置为操作E2Prom
    //internal_E2Write(addr,e2buf,32);
#if 1
   internal_E2Read(0,(uint8_t *)&gE2promState,sizeof(gE2promState));
   if(gE2promState.version  != 0xab)//查看是不是初始化了
   {
       gE2promState.systemState = eE2State_Default; //系统第一次上电默认状态
       gE2promState.version = 0xab;
       internal_E2Write(0,(uint8_t *)&gE2promState,sizeof(gE2promState));//初始化成默认值
   }
#endif
#if 0
   internal_E2Read(addr,e2buf,32);//读取内部e2prom 数据
   
   Uart_SendDataLen(e2buf,32);
   
   for(int i=0;i<32;i++)
       e2buf[i]++;
   
   internal_E2Write(addr,e2buf,32);
#endif
}

//内部e2读函数
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
        //EECON1 |= (0x01 << 0);//启动读标志位，RD由硬件清零
        //NOP();NOP();
        *dat++ = EEDATL;//读出寄存器数据
    }

}
//内部e2写函数
void internal_E2Write(uint8_t addr,uint8_t *dat,uint8_t len)
{
    uint8_t reg = 0;
    INTCON &= ~(0x01 << 7);//禁止全局中断
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
         //EECON1  |= (0x01 << 1);//设置WR位
         WR = 1;
         while(WR==1);
         WREN = 0;
         //while(PIR2 & (0x01 << 4));//等待E2PROM写操作完成
         //PIR2 &= ~(0x01 << 4);//清除完成标志，开始读写下一位
        
    }
    INTCON |= (0x01 << 7);//使能全局中断
    
}
//保存默认数据
void e2SavePara(e2Enum_t val)
{
    gE2promState.systemState = val;
    internal_E2Write(0,(uint8_t *)&gE2promState,sizeof(gE2promState));//初始化成默认值
}
