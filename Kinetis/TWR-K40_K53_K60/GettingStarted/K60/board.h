/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : board.h
 *    Description : Board specific definitions
 *
 *    History :
 *    1. Date        : 20, April 2011
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 **************************************************************************/

#include <NXP/MK60X256VMD100.h>

#ifndef __BOARD_H
#define __BOARD_H

#define SYS_CLK             25000000UL
#define TICK_PER_SEC        15UL

#define LED_SIM_SCGC_REG    SIM_SCGC5
#define LED_SIM_SCGC_MASK   SIM_SCGC5_PORTA_MASK
#define LED_SIM_SCGC_SHIFT  SIM_SCGC5_PORTA_SHIFT
#define LED_MASK            (1UL << 11)

#define LED_PCR_REG         PORTA_PCR11
#define LED_PSOR_REG        GPIOA_PSOR
#define LED_POER_REG        GPIOA_PDDR
#define LED_PTOR_REG        GPIOA_PTOR

#endif
