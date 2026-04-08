/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : lh7A40x_vic.c
 *    Description : Define API for VIC
 *
 *    History :
 *    1. Date        : May 15, 2006
 *       Author      : Stanimir Bonev
 *       Description : Porting for LH7A40x
 *
 *    $Revision: 34 $
 **************************************************************************/

#include "lh7A40x_vic.h"

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
  // Init VIC controller
  // Assign all interrupt channels to IRQ
  VIC_INTSEL_1 = VIC_INTSEL_2 = 0;
  // Diasable all interrupts
  VIC_INTENCLR_1 = VIC_INTENCLR_2 = 0xFFFFFFFF;
  // Clear all software interrutps
  VIC_SOFTINTCLR_1 = VIC_SOFTINTCLR_2 = 0xFFFFFFFF;
  // Clear interrupt
  VIC_VECTADDR_1 = VIC_VECTADDR_2 = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VIC_NVADDR_1 = VIC_NVADDR_2 = 0;

  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
  VIC_VAD0_1  = 0;
  VIC_VAD1_1  = 0;
  VIC_VAD2_1  = 0;
  VIC_VAD3_1  = 0;
  VIC_VAD4_1  = 0;
  VIC_VAD5_1  = 0;
  VIC_VAD6_1  = 0;
  VIC_VAD7_1  = 0;
  VIC_VAD8_1  = 0;
  VIC_VAD9_1  = 0;
  VIC_VAD10_1 = 0;
  VIC_VAD11_1 = 0;
  VIC_VAD12_1 = 0;
  VIC_VAD13_1 = 0;
  VIC_VAD14_1 = 0;
  VIC_VAD15_1 = 0;
  VIC_VAD0_2  = 0;
  VIC_VAD1_2  = 0;
  VIC_VAD2_2  = 0;
  VIC_VAD3_2  = 0;
  VIC_VAD4_2  = 0;
  VIC_VAD5_2  = 0;
  VIC_VAD6_2  = 0;
  VIC_VAD7_2  = 0;
  VIC_VAD8_2  = 0;
  VIC_VAD9_2  = 0;
  VIC_VAD10_2 = 0;
  VIC_VAD11_2 = 0;
  VIC_VAD12_2 = 0;
  VIC_VAD13_2 = 0;
  VIC_VAD14_2 = 0;
  VIC_VAD15_2 = 0;

  // Disable all vectored IRQ slots
  VIC_VECTCNTL0_1  = 0;
  VIC_VECTCNTL1_1  = 0;
  VIC_VECTCNTL2_1  = 0;
  VIC_VECTCNTL3_1  = 0;
  VIC_VECTCNTL4_1  = 0;
  VIC_VECTCNTL5_1  = 0;
  VIC_VECTCNTL6_1  = 0;
  VIC_VECTCNTL7_1  = 0;
  VIC_VECTCNTL8_1  = 0;
  VIC_VECTCNTL9_1  = 0;
  VIC_VECTCNTL10_1 = 0;
  VIC_VECTCNTL11_1 = 0;
  VIC_VECTCNTL12_1 = 0;
  VIC_VECTCNTL13_1 = 0;
  VIC_VECTCNTL14_1 = 0;
  VIC_VECTCNTL15_1 = 0;
  VIC_VECTCNTL0_2  = 0;
  VIC_VECTCNTL1_2  = 0;
  VIC_VECTCNTL2_2  = 0;
  VIC_VECTCNTL3_2  = 0;
  VIC_VECTCNTL4_2  = 0;
  VIC_VECTCNTL5_2  = 0;
  VIC_VECTCNTL6_2  = 0;
  VIC_VECTCNTL7_2  = 0;
  VIC_VECTCNTL8_2  = 0;
  VIC_VECTCNTL9_2  = 0;
  VIC_VECTCNTL10_2 = 0;
  VIC_VECTCNTL11_2 = 0;
  VIC_VECTCNTL12_2 = 0;
  VIC_VECTCNTL13_2 = 0;
  VIC_VECTCNTL14_2 = 0;
  VIC_VECTCNTL15_2 = 0;

}

/*************************************************************************
 * Function Name: VIC_GetIRQStatus
 * Parameters: VicController_t VicController
 * Return: unsigned int
 *
 * Description: Get IRQ Status of VIC. Return register VICIRQSTATUS's value.
 *              If some IRQ interrupt request is enabled, then the corresponding
 *    bit of VICIRQSTATUS is set.
 *
 *************************************************************************/
unsigned int VIC_GetIRQStatus(VicController_t VicController)
{
  return((VicController == Vic1)?VIC_IRQSTATUS_1:VIC_IRQSTATUS_2);
}

/*************************************************************************
 * Function Name: VIC_GetFIQStatus
 * Parameters: VicController_t VicController
 * Return: unsigned int
 *
 * Description: Get FIQ Status of VIC. Return register VICFIQSTATUS's value.
 *              If some FIQ interrupt request is enabled, then the corresponding
 *    bit of VICFIQSTATUS is set. If more that one interrupt request
 *              is assigned as FIQ, then invoking this function can decide which
 *              one or ones is/are the request source(s).
 *
 *************************************************************************/
unsigned int VIC_GetFIQStatus(VicController_t VicController)
{
  return((VicController == Vic1)?VIC_FIQSTATUS_1:VIC_FIQSTATUS_2);
}

/*************************************************************************
 * Function Name: VIC_EnableInt
 * Parameters: VicController_t VicController, unsigend int IntType
 * Return: void
 *
 * Description: Enable specific interrupt
 *
 *************************************************************************/
void VIC_EnableInt(VicController_t VicController, unsigned int IntType)
{
  if(VicController == Vic1)
  {
    VIC_INTEN_1 |= IntType;
  }
  else
  {
    VIC_INTEN_2 |= IntType;
  }
}

/*************************************************************************
 * Function Name: VIC_DisableInt
 * Parameters: VicController_t VicController, unsigned int IntType
 * Return: void
 *
 * Description: Disable specific interrupt
 *
 *************************************************************************/
void VIC_DisableInt(VicController_t VicController, unsigned int IntType)
{
  if(VicController == Vic1)
  {
    VIC_INTENCLR_1 |= IntType;
  }
  else
  {
    VIC_INTENCLR_2 |= IntType;
  }
}

/*************************************************************************
 * Function Name: VIC_EnableNonVectoredIRQ
 * Parameters:  VicController_t VicController, 
 *              pIRQSub - Non Vectored IRQ Sub address
 * Return: void
 *
 * Description:  Set VICDefVectAddr to be the IRQ Sub address.
 *
 *************************************************************************/
void VIC_EnableNonVectoredIRQ(VicController_t VicController, void(*pIRQSub)())
{
  if(VicController == Vic1)
  {
    VIC_NVADDR_1 = (unsigned int)pIRQSub;
  }
  else
  {
    VIC_NVADDR_2 = (unsigned int)pIRQSub;
  }
}

/*************************************************************************
 * Function Name: VIC_DisableNonVectoredIRQ
 * Parameters: VicController_t VicController
 * Return: void
 *
 * Description: set VICDefVectAddr to be reset value (NULL).
 *
 *************************************************************************/
void VIC_DisableNonVectoredIRQ(VicController_t VicController)
{
  if(VicController == Vic1)
  {
    VIC_NVADDR_1 = (unsigned long)NULL;
  }
  else
  {
    VIC_NVADDR_2 = (unsigned long)NULL;
  }
}

/*************************************************************************
 * Function Name: VIC_SetVectoredIRQ
 * Parameters:  VicController_t VicController, void(*pIRQSub)()
 *              VicIrqSlots_t VicIrqSlot
 *              unsigned int VicIntSouce
 *
 * Return: void
 *
 * Description:  Init vectored inerrutps
 *
 *************************************************************************/
void VIC_SetVectoredIRQ(VicController_t VicController,void(*pIRQSub)(), 
                        VicIrqSlots_t VicIrqSlot, unsigned int VicIntSource)
{
unsigned long volatile *pReg;
  // load base address of vectored address registers
  pReg = (VicController == Vic1)?&VIC_VAD0_1:&VIC_VAD0_2;
  // Set Address of callback function to corresponding Slot
  *(pReg+VicIrqSlot) = (unsigned long)pIRQSub;
  // load base address of ctrl registers
  pReg = (VicController == Vic1)?&VIC_VECTCNTL0_1:&VIC_VECTCNTL0_2;
  // Set source channel and enable the slot
  *(pReg+VicIrqSlot) = VicIntSource | 0x20;
  // Clear FIQ select bit
  if(VicController == Vic1)
  {
    VIC_INTSEL_1 &= ~(1<<VicIntSource);
  }
  else
  {
    VIC_INTSEL_2 &= ~(1<<VicIntSource);
  }
}
