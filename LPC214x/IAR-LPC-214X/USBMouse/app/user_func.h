/*************************************************************************
 *
 *    Used with ARM IAR C/C++ Compiler
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name      : user_func.h
 *    Description    :
 *
 *    History :
 *    1. Date        : June 13, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "includes.h"

#ifndef  __USER_FUNC_H
#define  __USER_FUNC_H

#define BUTT_EVENT_1    1
#define BUTT_EVENT_2    2
#define BUTT_FLAG_1     0x10
#define BUTT_FLAG_2     0x20
#define BUTT_DEB        2    /* 20ms */

/* Max Adc frequency [Hz]*/
#define MAX_ADC_FREQ      4500000

extern int TickSysFlag;

void ButtonsInit (void);
LPC_INT8U GetButtonsEvent (void);
void SetSysTickFlag (void* arg);
unsigned int ADC_Measure (void);
void ADC_Init (void);

#endif  /* __USER_FUNC_H */
