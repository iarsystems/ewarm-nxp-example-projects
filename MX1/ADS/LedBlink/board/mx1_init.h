/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : mx1_init.h
 *    Description : MX1 initialization module include file
 *
 *    History :
 *    1. Date        : 7 Feb. 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#ifndef __MX1_INIT_H
#define __MX1_INIT_H

// DBG UART (UART1) definitions
#define UART1_BAUD_RATE     115200

// Interrupt handlers table
extern VoidFpnt_t pIRQ_INTR_HANDLES[64];

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ Handler
 *
 *************************************************************************/
__arm __irq void irq_handler (void);

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: FIQ Handler
 *
 *************************************************************************/
__arm __fiq void fiq_handler (void);

/*************************************************************************
 * Function Name: AitcEnableInt
 * Parameters: VoidFpnt_t pIntrHandler,Int32U IntrInd,
 *						 Int32U IntrPtio, Boolean IntrType
 *
 * Return: none
 *
 * Description: Interrupt Handler registering
 *
 *************************************************************************/
void AitcEnableInt (VoidFpnt_t pIntrHandler,Int32U IntrInd,
                    Int32U IntrPtio, Boolean IntrType);

/*************************************************************************
 * Function Name: AitcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Interrupt controller init
 *
 *************************************************************************/
void AitcInit (void);

/*************************************************************************
 * Function Name: DbgUartGetChar
 * Parameters: none
 *
 * Return: Int8U
 *
 * Description: Receive char by UART 1
 *
 *************************************************************************/
Int8U DbgUartGetChar (void);

/*************************************************************************
 * Function Name: DbgUartPutChar
 * Parameters: Int8U Data
 *
 * Return: none
 *
 * Description: Send char by UART 1
 *
 *************************************************************************/
void DbgUartPutChar (Int8U Data);

/*************************************************************************
 * Function Name: DbgUartPrint
 * Parameters: Int8U const *pData
 *
 * Return: none
 *
 * Description: Send zero terminated massive
 *
 *************************************************************************/
void DbgUartPrint (Int8U const *pData);

/*************************************************************************
 * Function Name: DbgUartInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init UART1
 *
 *************************************************************************/
void DbgUartInit (void);

/*************************************************************************
 * Function Name: __low_level_init
 * Parameters: none
 *
 * Return: int
 *
 * Description: Low level init subroutine
 *
 *************************************************************************/
__arm int __low_level_init(void);

#endif // __MX1_INIT_H
