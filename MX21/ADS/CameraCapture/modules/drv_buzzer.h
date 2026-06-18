/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_buzzer.h
 *    Description : Touch screen  driver include file
 *
 *    History :
 *    1. Date        : 14 May, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#ifndef __DRV_BUZZER_H
#define __DRV_BUZZER_H

typedef enum
{
  ShortHighFreq = 0, LongHighFreq,
  ShortLowFreq, LongLowFreq,
} Beep_types_t;

/*************************************************************************
 * Function Name: Pwm_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: PWM interrupt handler
 *
 *************************************************************************/
void Pwm_Handler (void);

/*************************************************************************
 * Function Name: BuzzerInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init PWM
 *
 *************************************************************************/
void BuzzerInit (void);

/*************************************************************************
 * Function Name: Beep
 * Parameters: Int32 ToneType
 *
 * Return: Boolean TRUE - tone generation start
 *                 FALSE - error
 *
 * Description: Make Beep
 *
 *************************************************************************/
Boolean Beep (Int32U ToneType);

#endif // __DRV_BUZZER_H
