/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : drv_nvic.h
 *    Description : NXP LPC11A14 Nested Vectored Interrupt Controller
 *                  header file
 *    History :
 *    1. Date        : June 2011 
 *       Author      : Stoyan Choynev
 *       Description : initial revision
 *
 *    2. Date        : September 2011
 *       Author      : Atanas Uzunov
 *       Description : Changed function names to be identical with CMSIS.
 *                     Ported to LPC11A14   
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __NVIC_H
#define __NVIC_H
/** include files **/
#include <stdint.h>
/** definitions **/

/** public data **/
extern uint32_t CriticalSecCntr;

/** public functions **/
/*************************************************************************
 * Function Name: NVIC_EnableIRQ
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Enable interrupt at NVIC
 *
 *************************************************************************/
extern void NVIC_EnableIRQ(uint32_t IntNumber);

/*************************************************************************
 * Function Name: NVIC_DisableIRQ
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Disable interrupt at NVIC
 *
 *************************************************************************/
void NVIC_DisableIRQ(uint32_t IntNumber);

/*************************************************************************
 * Function Name: NVIC_ClearPendingIRQ
 * Parameters: IntNumber - Interrupt number
 * Return: void
 *
 * Description: Clear pending interrupt at NVIC
 *
 *************************************************************************/
 void NVIC_ClearPendingIRQ(uint32_t IntNumber);

/*************************************************************************
 * Function Name: NVIC_SetPriority
 * Parameters: IntNumber - Interrupt number, Priority - Interrupt priority
 * Return: void
 *
 * Description:Sets Interrupt priority
 *
 *************************************************************************/
void NVIC_SetPriority(uint32_t IntNumber, uint8_t Priority);

#endif  /* __NVIC_H */
