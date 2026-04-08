/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : drv_asic.c
 *    Description : ASIC driver
 *
 *    History :
 *    1. Date        : October 5, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#define ASIC_GLOBAL
#include "drv_asic.h"
IntrFunc_t IRQTable[64];
IntrFunc_t FIQTable[64];

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ interrupt handler
 *
 *************************************************************************/
__arm __irq void irq_handler(void)
{
  IRQTable[((ASIC_NIVECSR >> 16) & 0x3F)]();
}

/*************************************************************************
 * Function Name: ASIC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init ASIC
 *
 *************************************************************************/
void ASIC_Init(void)
{
  ASIC_INTCNTL_bit.FIDIS = 1;  // Disable all fast interrupts
  ASIC_INTCNTL_bit.NIDIS = 1;  // Disable all normal interrupts

  // Mask interrupt by priority
  ASIC_NIMASK = 0;
  ASIC_INTENABLEH = 0;
  ASIC_INTENABLEL = 0; // all interrupts are disable
  ASIC_INTTYPEH = 0;
  ASIC_INTTYPEL = 0;   // all interrupt lines are routed to the normal interrupt
}

/*************************************************************************
 * Function Name: ASIC_SetupInterrupt
 * Parameters: void(*pIntrSub)(), Boolean FastIntr,
 *             Int32U IntSouce, Int32U Priority
 *
 * Return: none
 *
 * Description: Setup the IRQ interrupt, and update the vector address
 *
 *************************************************************************/
void ASIC_SetupInterrupt(IntrFunc_t pIntrSub, Boolean FastIntr,
                    Int32U IntSouce, Int32U Priority)
{
  // Sanitary check
  if(IntSouce > 63)
  {
    return;
  }
  if(FastIntr)
  {
    // Fast interrupt
    FIQTable[IntSouce] = pIntrSub;
  }
  else
  {
    // Normal interrupt
    IRQTable[IntSouce] = pIntrSub;
    ASIC_SetIrqPriority(IntSouce,Priority);
  }
  ASIC_SetIntType(IntSouce,FastIntr);
}

/*************************************************************************
 * Function Name: ASIC_SetIntType
 * Parameters: Int32U intNum, Boolean FastIntr
 *
 * Return: none
 *
 * Description: This function specifies whether a normal or a fast
 * interrupt is generated for each of the interrupt lines
 *
 *************************************************************************/
void ASIC_SetIntType(Int32U intNum, Boolean FastIntr)
{
volatile Int32U * pReg = (volatile Int32U *)&ASIC_INTTYPEL;

  if(intNum >= 32)
  {
    pReg = (volatile Int32U *)&ASIC_INTTYPEH;
    intNum -= 32;
  }

  if(FastIntr)
  {
    *pReg |= (1UL << intNum);
  }
  else
  {
    *pReg &= ~(1UL << intNum);
  }
}

/*************************************************************************
 * Function Name: ASIC_SetIrqPriority
 * Parameters: Int32U intNum, Int32U PriLevel
 *
 * Return: none
 *
 * Description: This function is used to establish priority for
 * each of the 64 interrupts lines of the AITC.
 *
 *************************************************************************/
void  ASIC_SetIrqPriority(Int32U intNum, Int32U PriLevel)
{
  switch(intNum)
  {
  case 63:
    ASIC_NIPRIORITY7_bit.NIPR63 = PriLevel;
    break;
  case 62:
    ASIC_NIPRIORITY7_bit.NIPR62 = PriLevel;
    break;
  case 61:
    ASIC_NIPRIORITY7_bit.NIPR61 = PriLevel;
    break;
  case 60:
    ASIC_NIPRIORITY7_bit.NIPR60 = PriLevel;
    break;
  case 59:
    ASIC_NIPRIORITY7_bit.NIPR59 = PriLevel;
    break;
  case 58:
    ASIC_NIPRIORITY7_bit.NIPR58 = PriLevel;
    break;
  case 57:
    ASIC_NIPRIORITY7_bit.NIPR57 = PriLevel;
    break;
  case 56:
    ASIC_NIPRIORITY7_bit.NIPR56 = PriLevel;
    break;
  case 55:
    ASIC_NIPRIORITY6_bit.NIPR55 = PriLevel;
    break;
  case 54:
    ASIC_NIPRIORITY6_bit.NIPR54 = PriLevel;
    break;
  case 53:
    ASIC_NIPRIORITY6_bit.NIPR53 = PriLevel;
    break;
  case 52:
    ASIC_NIPRIORITY6_bit.NIPR52 = PriLevel;
    break;
  case 51:
    ASIC_NIPRIORITY6_bit.NIPR51 = PriLevel;
    break;
  case 50:
    ASIC_NIPRIORITY6_bit.NIPR50 = PriLevel;
    break;
  case 49:
    ASIC_NIPRIORITY6_bit.NIPR49 = PriLevel;
    break;
  case 48:
    ASIC_NIPRIORITY6_bit.NIPR48 = PriLevel;
    break;
  case 47:
    ASIC_NIPRIORITY5_bit.NIPR47 = PriLevel;
    break;
  case 46:
    ASIC_NIPRIORITY5_bit.NIPR46 = PriLevel;
    break;
  case 45:
    ASIC_NIPRIORITY5_bit.NIPR45 = PriLevel;
    break;
  case 44:
    ASIC_NIPRIORITY5_bit.NIPR44 = PriLevel;
    break;
  case 43:
    ASIC_NIPRIORITY5_bit.NIPR43 = PriLevel;
    break;
  case 42:
    ASIC_NIPRIORITY5_bit.NIPR42 = PriLevel;
    break;
  case 41:
    ASIC_NIPRIORITY5_bit.NIPR41 = PriLevel;
    break;
  case 40:
    ASIC_NIPRIORITY5_bit.NIPR40 = PriLevel;
    break;
  case 39:
    ASIC_NIPRIORITY4_bit.NIPR39 = PriLevel;
    break;
  case 38:
    ASIC_NIPRIORITY4_bit.NIPR38 = PriLevel;
    break;
  case 37:
    ASIC_NIPRIORITY4_bit.NIPR37 = PriLevel;
    break;
  case 36:
    ASIC_NIPRIORITY4_bit.NIPR36 = PriLevel;
    break;
  case 35:
    ASIC_NIPRIORITY4_bit.NIPR35 = PriLevel;
    break;
  case 34:
    ASIC_NIPRIORITY4_bit.NIPR34 = PriLevel;
    break;
  case 33:
    ASIC_NIPRIORITY4_bit.NIPR33 = PriLevel;
    break;
  case 32:
    ASIC_NIPRIORITY4_bit.NIPR32 = PriLevel;
    break;
  case 31:
    ASIC_NIPRIORITY3_bit.NIPR31 = PriLevel;
    break;
  case 30:
    ASIC_NIPRIORITY3_bit.NIPR30 = PriLevel;
    break;
  case 29:
    ASIC_NIPRIORITY3_bit.NIPR29 = PriLevel;
    break;
  case 28:
    ASIC_NIPRIORITY3_bit.NIPR28 = PriLevel;
    break;
  case 27:
    ASIC_NIPRIORITY3_bit.NIPR27 = PriLevel;
    break;
  case 26:
    ASIC_NIPRIORITY3_bit.NIPR26 = PriLevel;
    break;
  case 25:
    ASIC_NIPRIORITY3_bit.NIPR25 = PriLevel;
    break;
  case 24:
    ASIC_NIPRIORITY3_bit.NIPR24 = PriLevel;
    break;
  case 23:
    ASIC_NIPRIORITY2_bit.NIPR23 = PriLevel;
    break;
  case 22:
    ASIC_NIPRIORITY2_bit.NIPR22 = PriLevel;
    break;
  case 21:
    ASIC_NIPRIORITY2_bit.NIPR21 = PriLevel;
    break;
  case 20:
    ASIC_NIPRIORITY2_bit.NIPR20 = PriLevel;
    break;
  case 19:
    ASIC_NIPRIORITY2_bit.NIPR19 = PriLevel;
    break;
  case 18:
    ASIC_NIPRIORITY2_bit.NIPR18 = PriLevel;
    break;
  case 17:
    ASIC_NIPRIORITY2_bit.NIPR17 = PriLevel;
    break;
  case 16:
    ASIC_NIPRIORITY2_bit.NIPR16 = PriLevel;
    break;
  case 15:
    ASIC_NIPRIORITY1_bit.NIPR15 = PriLevel;
    break;
  case 14:
    ASIC_NIPRIORITY1_bit.NIPR14 = PriLevel;
    break;
  case 13:
    ASIC_NIPRIORITY1_bit.NIPR13 = PriLevel;
    break;
  case 12:
    ASIC_NIPRIORITY1_bit.NIPR12 = PriLevel;
    break;
  case 11:
    ASIC_NIPRIORITY1_bit.NIPR11 = PriLevel;
    break;
  case 10:
    ASIC_NIPRIORITY1_bit.NIPR10 = PriLevel;
    break;
  case 9:
    ASIC_NIPRIORITY1_bit.NIPR9 = PriLevel;
    break;
  case 8:
    ASIC_NIPRIORITY1_bit.NIPR8 = PriLevel;
    break;
  case 7:
    ASIC_NIPRIORITY0_bit.NIPR7 = PriLevel;
    break;
  case 6:
    ASIC_NIPRIORITY0_bit.NIPR6 = PriLevel;
    break;
  case 5:
    ASIC_NIPRIORITY0_bit.NIPR5 = PriLevel;
    break;
  case 4:
    ASIC_NIPRIORITY0_bit.NIPR4 = PriLevel;
    break;
  case 3:
    ASIC_NIPRIORITY0_bit.NIPR3 = PriLevel;
    break;
  case 2:
    ASIC_NIPRIORITY0_bit.NIPR2 = PriLevel;
    break;
  case 1:
    ASIC_NIPRIORITY0_bit.NIPR1 = PriLevel;
    break;
  case 0:
    ASIC_NIPRIORITY0_bit.NIPR0 = PriLevel;
    break;
  }
}
