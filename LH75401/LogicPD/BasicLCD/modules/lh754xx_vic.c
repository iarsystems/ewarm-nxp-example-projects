/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lh754xx_vic.h
 *    Description : Define API for VIC
 *
 *    History :
 *    4. Date        : May 5, 2006
 *       Author      : Stanimir Bonev
 *       Description : Porting for LH754xx
 *
 *    $Revision: 34 $
 **************************************************************************/

#include "lh754xx_vic.h"

/*************************************************************************
 * Function Name: VIC_Init
 * Parameters: void
 * Return: void
 *
 * Description: Initialize VIC
 *
 *************************************************************************/
void VIC_Init(void)
{
  // Assign all interrupt channels to IRQ
  VIC_IntSelect  =  0;
  // Disable all interrupts
  VIC_IntEnClear = 0xFFFFFFFF;
  // Clear all software interrupts
  VIC_SoftIntClear = 0xFFFFFFFF;
  // Clear interrupt
  VIC_VectAddr = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC_DefVectAddr = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC_VectAddr0  = 0;
  VIC_VectAddr1  = 0;
  VIC_VectAddr2  = 0;
  VIC_VectAddr3  = 0;
  VIC_VectAddr4  = 0;
  VIC_VectAddr5  = 0;
  VIC_VectAddr6  = 0;
  VIC_VectAddr7  = 0;
  VIC_VectAddr8  = 0;
  VIC_VectAddr9  = 0;
  VIC_VectAddr10 = 0;
  VIC_VectAddr11 = 0;
  VIC_VectAddr12 = 0;
  VIC_VectAddr13 = 0;
  VIC_VectAddr14 = 0;
  VIC_VectAddr15 = 0;

  // Disable all vectored IRQ slots
  VIC_VectCtrl0  = 0;
  VIC_VectCtrl1  = 0;
  VIC_VectCtrl2  = 0;
  VIC_VectCtrl3  = 0;
  VIC_VectCtrl4  = 0;
  VIC_VectCtrl5  = 0;
  VIC_VectCtrl6  = 0;
  VIC_VectCtrl7  = 0;
  VIC_VectCtrl8  = 0;
  VIC_VectCtrl9  = 0;
  VIC_VectCtrl10 = 0;
  VIC_VectCtrl11 = 0;
  VIC_VectCtrl12 = 0;
  VIC_VectCtrl13 = 0;
  VIC_VectCtrl14 = 0;
  VIC_VectCtrl15 = 0;
}

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
unsigned int VIC_GetIRQStatus(void)
{
  return(VIC_IRQStatus);
}

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
unsigned int VIC_GetFIQStatus(void)
{
  return(VIC_FIQStatus);
}

/*************************************************************************
 * Function Name: VIC_EnableInt
 * Parameters: lpc_uint32 IntType
 * Return: void
 *
 * Description: Enable specific interrupt
 *
 *************************************************************************/
void VIC_EnableInt(unsigned int IntType)
{
  VIC_IntEnable |= IntType;
}

/*************************************************************************
 * Function Name: VIC_DisableInt
 * Parameters: unsigned int IntType
 * Return: void
 *
 * Description: Disable specific interrupt
 *
 *************************************************************************/
void VIC_DisableInt(unsigned int IntType)
{
  VIC_IntEnClear |= IntType;
}

/*************************************************************************
 * Function Name: VIC_EnableNonVectoredIRQ
 * Parameters:  pIRQSub - Non Vectored IRQ Sub address
 * Return: void
 *
 * Description:  Set VICDefVectAddr to be the IRQ Sub address.
 *
 *************************************************************************/
void VIC_EnableNonVectoredIRQ(void(*pIRQSub)())
{
  VIC_DefVectAddr = (unsigned int)pIRQSub;
}

/*************************************************************************
 * Function Name: VIC_DisableNonVectoredIRQ
 * Parameters: void
 * Return: void
 *
 * Description: set VICDefVectAddr to be reset value (NULL).
 *
 *************************************************************************/
void VIC_DisableNonVectoredIRQ(void)
{
  VIC_DefVectAddr = (unsigned long)NULL;
}

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
void VIC_SetVectoredIRQ(void(*pIRQSub)(), VicIrqSlots_t VicIrqSlot, unsigned int VicIntSource)
{
unsigned long volatile *pReg;
  // load base address of vectored address registers
  pReg = &VIC_VectAddr0;
  // Set Address of callback function to corresponding Slot
  *(pReg+VicIrqSlot) = (unsigned long)pIRQSub;
  // load base address of ctrl registers
  pReg = &VIC_VectCtrl0;
  // Set source channel and enable the slot
  *(pReg+VicIrqSlot) = VicIntSource | 0x20;
  // Clear FIQ select bit
  VIC_IntSelect &= ~(1<<VicIntSource);
}
