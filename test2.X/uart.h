/* 
 * File:   uart.h
 * Author: USER
 *
 * Created on 2019年7月3日, 上午11:40
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "pic.h"
#include <htc.h>
#include <stdint.h>
   
void PIC_ConfigUart(void);
void Uart_SendStr(uint8_t *str);
void Uart_SendDataLen(uint8_t *str,uint16_t len);
void Uart_ShowInt(uint16_t intnum);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

