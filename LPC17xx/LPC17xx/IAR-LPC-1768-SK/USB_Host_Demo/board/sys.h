/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : sys.c
 *    Description : System module include file
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#ifndef __SYS_H
#define __SYS_H

#define FCCLK_FREQ 96000000

/*************************************************************************
 * Function Name: NVIC_IntEnable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Enable interrup at NVIC 
 *             
 *
 *************************************************************************/
extern void NVIC_IntEnable(Int32U IntNumber);

/*************************************************************************
 * Function Name: NVIC_IntDisable
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description: Disables interrup at NVIC 
 *              
 *
 *************************************************************************/

extern void NVIC_IntDisable(Int32U IntNumber);

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number
 * Return: void
 *
 * Description:Clear pending interrupt at NVIC
 *              
 *
 *************************************************************************/
extern void NVIC_ClrPend(Int32U IntNumber);

/*************************************************************************
 * Function Name: NVIC_ClrPend
 * Parameters: IntNumber - Interrup number, Interrupt Priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *              
 *
 *************************************************************************/
extern void NVIC_IntPri(Int32U IntNumber,Int8U Priority);

/*************************************************************************
 * Function Name: InitClock
 * Parameters: void
 * Return: void
 *
  * Description: Initialize PLL and clocks' dividers. Hclk - 288MHz,
 *              Fcckl = 96MHz
 *
 *************************************************************************/
void InitClock(void);

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: Int32U
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFsclk(void);

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: Int32U
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
Int32U SYS_GetFpclk(Int32U Periphery);

/*************************************************************************
 * Function Name: GpioInit
 * Parameters: void
 * Return: void
 *
 * Description: Reset all GPIO pins to default: primary function
 *
 *************************************************************************/
void GpioInit(void);

#endif // __SYS_H
