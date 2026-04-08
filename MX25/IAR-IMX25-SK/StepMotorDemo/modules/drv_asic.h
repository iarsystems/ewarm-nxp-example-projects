/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : drv_asic.h
 *    Description : ASIC driver include file
 *
 *    History :
 *    1. Date        : October 5, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 *
 **************************************************************************/
#include "arm_comm.h"

#include <NXP/iomcimx25.h>

#ifndef __DRV_ASIC_H
#define __DRV_ASIC_H

typedef enum _VectTableMode_t
{
  TableHighAddr = 0, TableLowAddr
}VectTableMode_t;

typedef void(*IntrFunc_t)(void);

/*************************************************************************
 * Function Name: ASIC_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init AITC
 *
 *************************************************************************/
void ASIC_Init(void);

/*************************************************************************
 * Function Name: ASIC_SetupIntr
 * Parameters: void(*pIntrSub)(), Boolean FastIntr,
 *             Int32U IntSouce, Int32U Priority
 *
 * Return: none
 *
 * Description: Setup the IRQ interrupt, and update the vector address
 *
 *************************************************************************/
void ASIC_SetupIntr(IntrFunc_t pIntrSub, Boolean FastIntr,
                    Int32U IntSouce, Int32U Priority);

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
void ASIC_SetIntType(Int32U intNum, Boolean FastIntr);

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
void  ASIC_SetIrqPriority(Int32U intNum, Int32U PriLevel);

/*************************************************************************
 * Function Name: ASIC_DisableIRQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable IRQ interrupts
 *
 *************************************************************************/
inline
void ASIC_DisableIRQ(void)
{
  INTCNTL_bit.NIDIS = 1;
}

/*************************************************************************
 * Function Name: ASIC_EnableIRQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable IRQ interrupts
 *
 *************************************************************************/
inline
void ASIC_EnableIRQ(void)
{
  INTCNTL_bit.NIDIS = 0;
}

/*************************************************************************
 * Function Name: ASIC_DisableFIQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable FIQ interrupts
 *
 *************************************************************************/
inline
void ASIC_DisableFIQ(void)
{
  INTCNTL_bit.FIDIS = 1;
}

/*************************************************************************
 * Function Name: ASIC_EnableFIQ
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable FIQ interrupts
 *
 *************************************************************************/
inline
void ASIC_EnableFIQ(void)
{
  INTCNTL_bit.FIDIS = 0;
}

/*************************************************************************
 * Function Name: ASIC_MaskIRQLevel
 * Parameters: Int32U maskLevel
 *
 * Return: none
 *
 * Description: Set mask interrupt level
 *
 *************************************************************************/
inline
void ASIC_MaskIRQLevel(Int32U maskLevel)
{
  NIMASK_bit.NIMASK = maskLevel;
}

/*************************************************************************
 * Function Name: ASIC_MaskIRQLevel
 * Parameters: Int32U maskLevel
 *
 * Return: none
 *
 * Description: Enable an interrupt source
 *
 *************************************************************************/
inline
void ASIC_EnableIntSource(Int32U intNum)
{
	INTENNUM_bit.ENNUM = intNum;
}

/*************************************************************************
 * Function Name: ASIC_MaskIRQLevel
 * Parameters: Int32U maskLevel
 *
 * Return: none
 *
 * Description: Disable an interrupt source
 *
 *************************************************************************/
inline
void ASIC_DisableIntSource(Int32U intNum)
{
	INTDISNUM_bit.DISNUM = intNum;
}

/*************************************************************************
 * Function Name: ASIC_FIQ_RestoreArb
 * Parameters: none
 *
 * Return: none
 *
 * Description: Restore normal arbitration priority of the ARM core
 *
 *************************************************************************/
inline
void ASIC_FIQ_RestoreArb(void)
{
}

#endif // __DRV_ASIC_H
