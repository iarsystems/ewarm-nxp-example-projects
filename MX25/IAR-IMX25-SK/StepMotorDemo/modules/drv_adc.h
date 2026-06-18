/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_adc.h
 *    Description : ADC driver module include file
 *
 *    History :
 *    1. Date        : February, 2 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

#ifndef __DRV_ADC_H
#define __DRV_ADC_H

/*************************************************************************
 * Function Name: AdcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init ADC
 *
 *************************************************************************/
void AdcInit (void);

/*************************************************************************
 * Function Name: VR_Measure
 * Parameters: none
 *
 * Return: Int16U
 *
 * Description: Return current voltage on WIPER input
 *
 *************************************************************************/
Int16U VR_Measure(void);

#endif // __DRV_TOUCH_SCR_H
