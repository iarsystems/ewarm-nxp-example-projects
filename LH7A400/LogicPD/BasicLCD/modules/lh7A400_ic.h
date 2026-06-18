/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lh7A400_ic.h
 *    Description : Define API for IC
 *
 *    History :
 *    1. Date        : May 18, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/

#include "includes.h"

#ifndef __LH7A400_IC_H
#define __LH7A400_IC_H

#define IRQ_FLAG        0x80

extern VoidFpnt_t IntrHanders[32];

/* Declare API functions */
/*************************************************************************
 * Function Name: IC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize IC
 *
 *************************************************************************/
void IC_Init(void);

/*************************************************************************
 * Function Name: IC_GetIRQStatus
 * Parameters: none
 * Return: unsigned int
 *
 * Description: Get IRQ interrupts enable status of IC.
 *
 *************************************************************************/
unsigned int IC_GetIRQStatus(void);

/*************************************************************************
 * Function Name: IC_EnableInt
 * Parameters: unsigned int IntType
 * Return: void
 *
 * Description: Enable specific interrupt
 *
 *************************************************************************/
void IC_EnableInt(unsigned int IntType);

/*************************************************************************
 * Function Name: IC_DisableInt
 * Parameters: unsigned int IntType
 * Return: void
 *
 * Description: Disable specific interrupt
 *
 *************************************************************************/
void IC_DisableInt(unsigned int IntType);

/*************************************************************************
 * Function Name: IC_SetVectoredIRQ
 * Parameters:  void(*pIRQSub)()
 *              unsigned int IcIrqSlot
 *
 * Return: void
 *
 * Description:  Init vectored interrupts
 *
 *************************************************************************/
void IC_SetVectoredIRQ(void(*pIRQSub)(),unsigned int IcIrqSlot);

#endif // __LH7A400_VIC_H
