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

#include <NXP/MK53N512CMD100.h>

#ifndef __BOARD_H
#define __BOARD_H

#define SYS_CLK             25000000UL
#define TICK_PER_SEC        15UL

#define LED_SIM_SCGC_REG    SIM_SCGC5
#define LED_SIM_SCGC_MASK   SIM_SCGC5_PORTC_MASK
#define LED_SIM_SCGC_SHIFT  SIM_SCGC5_PORTC_SHIFT
#define LED_MASK            (1UL << 7)

#define LED_PCR_REG         PORTC_PCR7
#define LED_PSOR_REG        GPIOC_PSOR
#define LED_POER_REG        GPIOC_PDDR
#define LED_PTOR_REG        GPIOC_PTOR

#endif
