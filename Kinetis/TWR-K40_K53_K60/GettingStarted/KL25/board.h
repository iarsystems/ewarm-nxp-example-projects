/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : board.h
 *    Description : Board specific definitions
 *
 *    History :
 *    1. Date        : 29, May 2012
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 **************************************************************************/
#include <NXP/MKL25Z4.h>

#ifndef __BOARD_H
#define __BOARD_H

#define SYS_CLK             8000000UL
#define TICK_PER_SEC        15UL


#define LED_SIM_SCGC_REG    SIM_SCGC5
#define LED_SIM_SCGC_MASK   SIM_SCGC5_PORTA_MASK
#define LED_SIM_SCGC_SHIFT  SIM_SCGC5_PORTA_SHIFT
#define LED_MASK            (1UL << 5)

#define LED_PCR_REG         PORTA_PCR5
#define LED_PSOR_REG        FGPIOA_PSOR
#define LED_POER_REG        FGPIOA_PDDR
#define LED_PTOR_REG        FGPIOA_PTOR

#endif
