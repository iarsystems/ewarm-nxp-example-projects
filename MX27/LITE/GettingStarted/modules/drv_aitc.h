/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_aitc.h
 *    Description : AICT driver include file
 *
 *    History :
 *    1. Date        : May 1, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 *
 **************************************************************************/
#include "arm_comm.h"

#include <NXP/iomcimx27.h>

#ifndef __DRV_AICT_H
#define __DRV_AICT_H

#define AITC_NOMASKED_INTR  0x10

typedef enum _VectTableMode_t
{
  TableHighAddr = 0, TableLowAddr
}VectTableMode_t;

typedef void(*IntrFunc_t)(void);

typedef struct _AITC_Ctrl_t
{
  // table offset value must be 32 bit alignment and between 0x24 and 0xF00
  Int32U VectTblOffset;
  // normal interrupt mask level (0x10–0x1F = Do not disable any normal interrupts)
  Int32U IntrPriorityMask;
  // ABFLAG bit is sticky and requires a write of 1 to clear the bit
  Int32U PrioritySticky       : 1;
  // Enable/ Disable normal interrupt flag increases the bus arbitration priority
  // of the ARM  core to decrease the latency of the interrupt service routine
  Int32U NormIntrRisePriority : 1;
  // Enable/ Disable fast interrupt flag increases the bus arbitration priority
  // of the ARM  core to decrease the latency of the interrupt service routine
  Int32U FastIntrRisePriority : 1;
  // Vector table map address 0 - 0xFFFFF000
  //                          1 - depend of the VectTblOffset's value
  Int32U VectTbl              : 1;
} AITC_Ctrl_t, * pAITC_Ctrl_t;

/*************************************************************************
 * Function Name: AITC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init AITC
 *
 *************************************************************************/
void AITC_Init(pAITC_Ctrl_t pAICT_Ctrl);

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
                    Int32U IntSouce, Int32U Priority);

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
void AITC_SetIntType(Int32U intNum, Boolean FastIntr);

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
void  AITC_SetIrqPriority(Int32U intNum, Int32U PriLevel);

/*************************************************************************
 * Function Name: AITC_DisableIRQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable IRQ interrupts
 *
 *************************************************************************/
inline
void AITC_DisableIRQ(void)
{
  INTCNTL_bit.NIDIS = 1;
}

/*************************************************************************
 * Function Name: AITC_EnableIRQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable IRQ interrupts
 *
 *************************************************************************/
inline
void AITC_EnableIRQ(void)
{
  INTCNTL_bit.NIDIS = 0;
}

/*************************************************************************
 * Function Name: AITC_DisableFIQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable FIQ interrupts
 *
 *************************************************************************/
inline
void AITC_DisableFIQ(void)
{
  INTCNTL_bit.FIDIS = 1;
}

/*************************************************************************
 * Function Name: AITC_EnableFIQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable FIQ interrupts
 *
 *************************************************************************/
inline
void AITC_EnableFIQ(void)
{
  INTCNTL_bit.FIDIS = 0;
}

/*************************************************************************
 * Function Name: AITC_MaskIRQLevel
 * Parameters: Int32U maskLevel
 *
 * Return: none
 *
 * Description: Set mask interrupt level
 *
 *************************************************************************/
inline
void AITC_MaskIRQLevel(Int32U maskLevel)
{
  NIMASK_bit.NIMASK = maskLevel;
}

/*************************************************************************
 * Function Name: AITC_MaskIRQLevel
 * Parameters: Int32U maskLevel
 *
 * Return: none
 *
 * Description: Enable an interrupt source
 *
 *************************************************************************/
inline
void AITC_EnableIntSource(Int32U intNum)
{
	INTENNUM_bit.ENNUM = intNum;
}

/*************************************************************************
 * Function Name: AITC_MaskIRQLevel
 * Parameters: Int32U maskLevel
 *
 * Return: none
 *
 * Description: Disable an interrupt source
 *
 *************************************************************************/
inline
void AITC_DisableIntSource(Int32U intNum)
{
	INTDISNUM_bit.DISNUM = intNum;
}

/*************************************************************************
 * Function Name: AITC_FIQ_RestoreArb
 * Parameters: none
 *
 * Return: none
 *
 * Description: Restore normal arbitration priority of the ARM core
 *
 *************************************************************************/
inline
void AITC_FIQ_RestoreArb(void)
{
}

#endif // __DRV_AICT_H
