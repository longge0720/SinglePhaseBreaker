/* 
 * File:   internal_e2prom.h
 * Author: USER
 *
 * Created on 2020��4��26��, ����11:41
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
    uint8_t version;        //����汾
    uint8_t systemState;    //ϵͳ״̬,������Զ���բ,��һ�±�ǡ���բʱ��ǣ���բʱһ����ǡ�
    uint8_t phaseState;     //�����״̬
}e2State_t;       //ϵͳ״̬����ṹ��

extern e2State_t gE2promState;

void internal_E2promInit(void);
void internal_E2Read(uint8_t addr,uint8_t *dat,uint8_t len);
void internal_E2Write(uint8_t addr,uint8_t *dat,uint8_t len);
void e2SavePara(e2Enum_t val);
#ifdef	__cplusplus
}
#endif

#endif	/* INTERNAL_E2PROM_H */

