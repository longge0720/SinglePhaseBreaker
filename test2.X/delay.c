#include "delay.h"

static uint16_t delay_time = 0;

void Delay_MS(uint16_t ms)
{
    delay_time = ms;
    while(delay_time != 0);//�ȴ���ʱʱ�䵽��
}

void Delay_Scan(void)
{
    if(delay_time > 0)
    {
        delay_time--;
    }
}
