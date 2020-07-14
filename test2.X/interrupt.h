/* 
 * File:   interrupt.h
 * Author: USER
 *
 * Created on 2019��3��14��, ����10:07
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <stdlib.h>
#include "pic.h"
#include <htc.h>
    
    
void PIC_EnableInterrupt(void);
void PIC_DisableInterrupt(void);


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPT_H */

