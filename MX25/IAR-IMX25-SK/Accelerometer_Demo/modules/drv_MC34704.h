/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : drv_MC34704.h
 *    Description : MC34704 driver include file
 *
 *    History :
 *    1. Date        : 16 October, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "board.h"

#ifndef __DRV_MC34704_H
#define __DRV_MC347041_H

#define MC34704_SLAVE_ADDR  0xA8

typedef enum _MC34704_Reg_t
{
  GENERAL1 = 1, GENERAL2, GENERAL3,
  VGSET1, VGSET2,
  REG2SET1, REG2SET2,
  REG3SET1, REG3SET2,
  REG4SET1, REG4SET2,
  REG5SET1, REG5SET2, REG5SET3,
  REG6SET1, REG6SET2, REG6SET3,
  REG7SET1, REG7SET2, REG7SET3,
  REG8SET1, REG8SET2, REG8SET3,
  FAULTS, I2CSET1,
  REG3DAC = 0x49,
  REG7CR0 = 0x58, REG7DAC
} MC34704_Reg_t;

Boolean MC34704_Init (void);

#endif // __DRV_MC34704_H
