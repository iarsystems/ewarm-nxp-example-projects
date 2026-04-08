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

#include <NXP/MKE15D7.h>

#ifndef __BOARD_H
#define __BOARD_H

#define SYS_CLK             72000000UL
#define TICK_PER_SEC        15UL

#define LED_SIM_SCGC_REG    SIM_SCGC6
#define LED_SIM_SCGC_MASK   SIM_SCGC6_PORTE_MASK
#define LED_SIM_SCGC_SHIFT  SIM_SCGC6_PORTE_SHIFT
#define LED_MASK            (1UL << 6)

#define LED_PCR_REG         PORTE_PCR6
#define LED_PSOR_REG        GPIOE_PSOR
#define LED_POER_REG        GPIOE_PDDR
#define LED_PTOR_REG        GPIOE_PTOR

#endif
