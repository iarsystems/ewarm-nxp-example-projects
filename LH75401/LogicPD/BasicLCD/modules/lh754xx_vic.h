/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : lh754xx_vic.h
 *    Description :
 *
 *    History :
 *    1. Date        : May 5, 2006
 *       Author      : Stanimir Bonev
 *       Description : Porting for LH754xx
 *
 *    $Revision: 34 $
 **************************************************************************/

#ifndef __LH574XX_VIC_H
#define __LH574XX_VIC_H

#include <intrinsics.h>
#include <NXP/iolh75401.h>
#include "arm_comm.h"
#include "board.h"

#define IRQ_FLAG        0x80

typedef enum {
  VIC_Slot0 = 0,  // high priority
  VIC_Slot1,VIC_Slot2,VIC_Slot3,VIC_Slot4,VIC_Slot5,VIC_Slot6,VIC_Slot7,VIC_Slot8,
  VIC_Slot9,VIC_Slot10,VIC_Slot11,VIC_Slot12,VIC_Slot13,VIC_Slot14,VIC_Slot15
}VicIrqSlots_t;

/* Declare API functions */
/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void VIC_Init(void);

/*************************************************************************
 * Function Name: VIC_GetIRQStatus
 * Parameters: void
 * Return: unsigned int
 *
 * Description: Get IRQ Status of VIC. Return register VICIRQSTATUS's value.
 *              If some IRQ interrupt request is enabled, then the corresponding
 *    bit of VICIRQSTATUS is set.
 *
 *************************************************************************/
unsigned int VIC_GetIRQStatus(void);

/*************************************************************************
 * Function Name: VIC_GetFIQStatus
 * Parameters: void
 * Return: unsigned int
 *
 * Description: Get FIQ Status of VIC. Return register VICFIQSTATUS's value.
 *              If some FIQ interrupt request is enabled, then the corresponding
 *    bit of VICFIQSTATUS is set. If more that one interrupt request
 *              is assigned as FIQ, then invoking this function can decide which
 *              one or ones is/are the request source(s).
 *
 *************************************************************************/
unsigned int VIC_GetFIQStatus(void);

/*************************************************************************
 * Function Name: VIC_EnableInt
 * Parameters: lpc_uint32 IntType
 * Return: void
 *
 * Description: Enable specific interrupt
 *
 *************************************************************************/
void VIC_EnableInt(unsigned int IntType);

/*************************************************************************
 * Function Name: VIC_DisableInt
 * Parameters: unsigned int IntType
 * Return: void
 *
 * Description: Disable specific interrupt
 *
 *************************************************************************/
void VIC_DisableInt(unsigned int IntType);

/*************************************************************************
 * Function Name: VIC_EnableNonVectoredIRQ
 * Parameters:  pIRQSub - Non Vectored IRQ Sub address
 * Return: void
 *
 * Description:  Set VICDefVectAddr to be the IRQ Sub address.
 *
 *************************************************************************/
void VIC_EnableNonVectoredIRQ(void(*pIRQSub)());

/*************************************************************************
 * Function Name: VIC_DisableNonVectoredIRQ
 * Parameters: void
 * Return: void
 *
 * Description: set VICDefVectAddr to be reset value (NULL).
 *
 *************************************************************************/
void VIC_DisableNonVectoredIRQ(void);

/*************************************************************************
 * Function Name: VIC_SetVectoredIRQ
 * Parameters:  void(*pIRQSub)()
 *              VicIrqSlots_t VicIrqSlot
 *              unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Init vectored interrupts
 *
 *************************************************************************/
void VIC_SetVectoredIRQ(void(*pIRQSub)(), VicIrqSlots_t VicIrqSlot,
                        unsigned int VicIntSource);

#endif // __LH574XX_VIC_H
