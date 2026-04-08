/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lh7A400_ic.c
 *    Description : Define API for IC
 *
 *    History :
 *    1. Date        : May 18, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 34 $
 **************************************************************************/

#include "lh7A400_ic.h"

__no_init VoidFpnt_t IntrHanders[32];

/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void IC_Init(void)
{
  // Init VIC controller
  IC_INTENC = 0xFF;  // Disable all interrupts
  for(int i = 0; i < sizeof(IntrHanders)/sizeof(VoidFpnt_t); ++i)
  {
    IntrHanders[i] = NULL;
  }
}

/*************************************************************************
 * Function Name: IC_GetIRQStatus
 * Parameters: none
 * Return: unsigned int
 *
 * Description: Get IRQ interrupts enable status of IC.
 *
 *************************************************************************/
unsigned int IC_GetIRQStatus(void)
{
  return(IC_INTSR);
}

/*************************************************************************
 * Function Name: IC_EnableInt
 * Parameters: unsigned int IntType
 * Return: void
 *
 * Description: Enable specific interrupt
 *
 *************************************************************************/
void IC_EnableInt(unsigned int IntType)
{
  IC_INTENS = IntType;
}

/*************************************************************************
 * Function Name: IC_DisableInt
 * Parameters: unsigned int IntType
 * Return: void
 *
 * Description: Disable specific interrupt
 *
 *************************************************************************/
void IC_DisableInt(unsigned int IntType)
{
  IC_INTENC = IntType;
}

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
void IC_SetVectoredIRQ(void(*pIRQSub)(),unsigned int IcIrqSlot)
{
  if(IcIrqSlot < (sizeof(IntrHanders)/sizeof(VoidFpnt_t)))
  {
    IntrHanders[IcIrqSlot] = pIRQSub;
  }
}
