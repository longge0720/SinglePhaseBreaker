/* 
 * File:   lowpower.h
 * Author: USER
 *
 * Created on 2019年11月18日, 上午9:13
 */

#ifndef LOWPOWER_H
#define	LOWPOWER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "define.h"
    
void PowerSwitch_Init(void);
void PowerSwitch_Disable(void);
void PowerSwitch_Enable(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LOWPOWER_H */

