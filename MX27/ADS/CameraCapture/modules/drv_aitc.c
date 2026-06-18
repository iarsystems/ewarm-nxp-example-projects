/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_aitc.c
 *    Description : AITC driver
 *
 *    History :
 *    1. Date        : May 1, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#define AITC_GLOBAL
#include "drv_aitc.h"
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
  IRQTable[((NIVECSR >> 16) & 0x3F)]();
}

/*************************************************************************
 * Function Name: AITC_Init
 * Parameters: pAITC_Ctrl_t pAITC_Ctrl
 *
 * Return: none
 *
 * Description: Init AITC
 *
 *************************************************************************/
void AITC_Init(pAITC_Ctrl_t pAITC_Ctrl)
{
  INTCNTL_bit.FIDIS = 1;  // Disable all fast interrupts
  INTCNTL_bit.NIDIS = 1;  // Disable all normal interrupts

  // Normal interrupt priority control
  INTCNTL_bit.NIAD  = pAITC_Ctrl->NormIntrRisePriority;
  // Fast interrupt priority control
  INTCNTL_bit.FIAD  = pAITC_Ctrl->FastIntrRisePriority;
  // Vector table mapping
  INTCNTL_bit.MD    = pAITC_Ctrl->VectTbl;
  if(pAITC_Ctrl->VectTbl)
  {
    INTCNTL_bit.POINTER = pAITC_Ctrl->VectTblOffset >> 2;
  }

  // Mask interrupt by priority
  NIMASK = pAITC_Ctrl->IntrPriorityMask;
  INTENABLEH = 0;
  INTENABLEL = 0; // all interrupts are disable
  INTTYPEH = 0;
  INTTYPEL = 0;   // all interrupt lines are routed to the normal interrupt
}

/*************************************************************************
 * Function Name: AITC_SetupIntr
 * Parameters: void(*pIntrSub)(), Boolean FastIntr,
 *             Int32U IntSouce, Int32U Priority
 *
 * Return: none
 *
 * Description: Setup the IRQ interrupt, and update the vector address
 *
 *************************************************************************/
void AITC_SetupIntr(IntrFunc_t pIntrSub, Boolean FastIntr,
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
    AITC_SetIrqPriority(IntSouce,Priority);
  }
  AITC_SetIntType(IntSouce,FastIntr);
}

/*************************************************************************
 * Function Name: AITC_SetIntType
 * Parameters: Int32U intNum, Boolean FastIntr
 *
 * Return: none
 *
 * Description: This function specifies whether a normal or a fast
 * interrupt is generated for each of the interrupt lines
 *
 *************************************************************************/
void AITC_SetIntType(Int32U intNum, Boolean FastIntr)
{
volatile Int32U * pReg = (volatile Int32U *)&INTTYPEL;

  if(intNum >= 32)
  {
    pReg = (volatile Int32U *)&INTTYPEH;
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
 * Function Name: AITC_SetIrqPriority
 * Parameters: Int32U intNum, Int32U PriLevel
 *
 * Return: none
 *
 * Description: This function is used to establish priority for
 * each of the 64 interrupts lines of the AITC.
 *
 *************************************************************************/
void  AITC_SetIrqPriority(Int32U intNum, Int32U PriLevel)
{
  switch(intNum)
  {
  case 63:
    NIPRIORITY7_bit.NIPR63 = PriLevel;
    break;
  case 62:
    NIPRIORITY7_bit.NIPR62 = PriLevel;
    break;
  case 61:
    NIPRIORITY7_bit.NIPR61 = PriLevel;
    break;
  case 60:
    NIPRIORITY7_bit.NIPR60 = PriLevel;
    break;
  case 59:
    NIPRIORITY7_bit.NIPR59 = PriLevel;
    break;
  case 58:
    NIPRIORITY7_bit.NIPR58 = PriLevel;
    break;
  case 57:
    NIPRIORITY7_bit.NIPR57 = PriLevel;
    break;
  case 56:
    NIPRIORITY7_bit.NIPR56 = PriLevel;
    break;
  case 55:
    NIPRIORITY6_bit.NIPR55 = PriLevel;
    break;
  case 54:
    NIPRIORITY6_bit.NIPR54 = PriLevel;
    break;
  case 53:
    NIPRIORITY6_bit.NIPR53 = PriLevel;
    break;
  case 52:
    NIPRIORITY6_bit.NIPR52 = PriLevel;
    break;
  case 51:
    NIPRIORITY6_bit.NIPR51 = PriLevel;
    break;
  case 50:
    NIPRIORITY6_bit.NIPR50 = PriLevel;
    break;
  case 49:
    NIPRIORITY6_bit.NIPR49 = PriLevel;
    break;
  case 48:
    NIPRIORITY6_bit.NIPR48 = PriLevel;
    break;
  case 47:
    NIPRIORITY5_bit.NIPR47 = PriLevel;
    break;
  case 46:
    NIPRIORITY5_bit.NIPR46 = PriLevel;
    break;
  case 45:
    NIPRIORITY5_bit.NIPR45 = PriLevel;
    break;
  case 44:
    NIPRIORITY5_bit.NIPR44 = PriLevel;
    break;
  case 43:
    NIPRIORITY5_bit.NIPR43 = PriLevel;
    break;
  case 42:
    NIPRIORITY5_bit.NIPR42 = PriLevel;
    break;
  case 41:
    NIPRIORITY5_bit.NIPR41 = PriLevel;
    break;
  case 40:
    NIPRIORITY5_bit.NIPR40 = PriLevel;
    break;
  case 39:
    NIPRIORITY4_bit.NIPR39 = PriLevel;
    break;
  case 38:
    NIPRIORITY4_bit.NIPR38 = PriLevel;
    break;
  case 37:
    NIPRIORITY4_bit.NIPR37 = PriLevel;
    break;
  case 36:
    NIPRIORITY4_bit.NIPR36 = PriLevel;
    break;
  case 35:
    NIPRIORITY4_bit.NIPR35 = PriLevel;
    break;
  case 34:
    NIPRIORITY4_bit.NIPR34 = PriLevel;
    break;
  case 33:
    NIPRIORITY4_bit.NIPR33 = PriLevel;
    break;
  case 32:
    NIPRIORITY4_bit.NIPR32 = PriLevel;
    break;
  case 31:
    NIPRIORITY3_bit.NIPR31 = PriLevel;
    break;
  case 30:
    NIPRIORITY3_bit.NIPR30 = PriLevel;
    break;
  case 29:
    NIPRIORITY3_bit.NIPR29 = PriLevel;
    break;
  case 28:
    NIPRIORITY3_bit.NIPR28 = PriLevel;
    break;
  case 27:
    NIPRIORITY3_bit.NIPR27 = PriLevel;
    break;
  case 26:
    NIPRIORITY3_bit.NIPR26 = PriLevel;
    break;
  case 25:
    NIPRIORITY3_bit.NIPR25 = PriLevel;
    break;
  case 24:
    NIPRIORITY3_bit.NIPR24 = PriLevel;
    break;
  case 23:
    NIPRIORITY2_bit.NIPR23 = PriLevel;
    break;
  case 22:
    NIPRIORITY2_bit.NIPR22 = PriLevel;
    break;
  case 21:
    NIPRIORITY2_bit.NIPR21 = PriLevel;
    break;
  case 20:
    NIPRIORITY2_bit.NIPR20 = PriLevel;
    break;
  case 19:
    NIPRIORITY2_bit.NIPR19 = PriLevel;
    break;
  case 18:
    NIPRIORITY2_bit.NIPR18 = PriLevel;
    break;
  case 17:
    NIPRIORITY2_bit.NIPR17 = PriLevel;
    break;
  case 16:
    NIPRIORITY2_bit.NIPR16 = PriLevel;
    break;
  case 15:
    NIPRIORITY1_bit.NIPR15 = PriLevel;
    break;
  case 14:
    NIPRIORITY1_bit.NIPR14 = PriLevel;
    break;
  case 13:
    NIPRIORITY1_bit.NIPR13 = PriLevel;
    break;
  case 12:
    NIPRIORITY1_bit.NIPR12 = PriLevel;
    break;
  case 11:
    NIPRIORITY1_bit.NIPR11 = PriLevel;
    break;
  case 10:
    NIPRIORITY1_bit.NIPR10 = PriLevel;
    break;
  case 9:
    NIPRIORITY1_bit.NIPR9 = PriLevel;
    break;
  case 8:
    NIPRIORITY1_bit.NIPR8 = PriLevel;
    break;
  case 7:
    NIPRIORITY0_bit.NIPR7 = PriLevel;
    break;
  case 6:
    NIPRIORITY0_bit.NIPR6 = PriLevel;
    break;
  case 5:
    NIPRIORITY0_bit.NIPR5 = PriLevel;
    break;
  case 4:
    NIPRIORITY0_bit.NIPR4 = PriLevel;
    break;
  case 3:
    NIPRIORITY0_bit.NIPR3 = PriLevel;
    break;
  case 2:
    NIPRIORITY0_bit.NIPR2 = PriLevel;
    break;
  case 1:
    NIPRIORITY0_bit.NIPR1 = PriLevel;
    break;
  case 0:
    NIPRIORITY0_bit.NIPR0 = PriLevel;
    break;
  }
}
