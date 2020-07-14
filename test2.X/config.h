/* 
 * File:   config.h
 * Author: USER
 *
 * Created on 2019年11月8日, 上午9:14
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
extern uint8_t debugbuf[20];

#include "uart.h"
#include "stdio.h"
#include "motor.h"
#define DBG_UART_SENDLEN       Uart_SendStrLen        //Uart_SendDataLen
#define DBG_UART_SENDSTR       Uart_SendStr
#define DBG_SHOW_INT          Uart_ShowInt
#define DBG_PRINTF             //printf
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

