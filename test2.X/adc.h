/* 
 * File:   adc.h
 * Author: USER
 *
 * Created on 2019年5月5日, 上午9:20
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
#include <stdint.h>
typedef enum
{
    eADC_AN0 = 0,
    eADC_AN1 = 1,
    eADC_AN2 = 2,
    eADC_AN3 = 3,
    eADC_AN4 = 4,
    eADC_AN5 = 5,
    eADC_AN6 = 6,
    eADC_AN7 = 7,
    eADC_Temperature = 29,      //温度指示器
    eADC_DACOutput = 30,        //DAC输出
    eADC_FVRBuffer1 = 31,       //FVR 缓冲1输出
}ADCChnel_Enumdef;

void ADC_Init(void);
uint16_t Get_ADCValue(ADCChnel_Enumdef channel);
#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

