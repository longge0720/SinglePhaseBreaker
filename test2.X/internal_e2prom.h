/* 
 * File:   internal_e2prom.h
 * Author: USER
 *
 * Created on 2020年4月26日, 上午11:41
 */

#ifndef INTERNAL_E2PROM_H
#define	INTERNAL_E2PROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum
{
    eE2State_Default = 0,
    eE2State_AutoOpen ,
    eE2State_AutoClose,
    eE2State_ManOpen,
    eE2State_ManClose,
} e2Enum_t;

typedef struct
{
    uint8_t version;        //软件版本
    uint8_t systemState;    //系统状态,如果是自动分闸,做一下标记。分闸时标记，合闸时一个标记。
    uint8_t phaseState;     //零火线状态
}e2State_t;       //系统状态定义结构体

extern e2State_t gE2promState;

void internal_E2promInit(void);
void internal_E2Read(uint8_t addr,uint8_t *dat,uint8_t len);
void internal_E2Write(uint8_t addr,uint8_t *dat,uint8_t len);
void e2SavePara(e2Enum_t val);
#ifdef	__cplusplus
}
#endif

#endif	/* INTERNAL_E2PROM_H */

