/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : lpc288x_low_init.h
 *    Description : Low level init include file
 *
 *    History :
 *    1. Date        : 9, January 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include <NXP/iolpc2888.h>
#include "arm_comm.h"
#include "board.h"

#ifndef __LPC288x_LOW_INIT_H
#define __LPC288x_LOW_INIT_H

typedef enum _SectionInClockSelect_t
{
  LF_Osc = 0, HF_Osc, MCI_ClkPin, DAI_BCLK_ClkPin, DAI_WS_ClkPin,
  HighPLL = 7, MainPLL,
} SectionInClockSelect_t;

typedef struct _IntrVecTable_t
{
  VoidFpnt_t  pIntrFunc;
  Int32U      IntrPriority;
} IntrVecTable_t, *pIntrVecTable_t;

extern IntrVecTable_t IrqTable[31];

/*************************************************************************
 * Function Name: LPC288x_SectionClock
 * Parameters: volatile long * pReg, SectionInClockSelect_t Mode
 *
 * Return: void
 *
 * Description: This function is used for change of the section input clock
 *
 *************************************************************************/
void LPC288x_SectionClockSelect(volatile unsigned long * pReg, Int32U Mode);

#endif // __LPC288x_LOW_INIT_H
