/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 5880 $
 **
 ***************************************************************************/
#include <NXP/iomcimx25.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

/* OSC [MHz] */
#define FOSC1               26MHZ
#define FOSC2               327680HZ

/* Core clk [MHz]  266 MHz */
#define MPLL_CLK            266MHZ
#define FCLK                266MHZ
#define HCLK                133MHZ

/* GPT1 Tick per second*/
#define GPT1_TICK_PER_SEC   20

#define LED0                (1UL<< 0)
#define LED1                (1UL<< 1)
#define LED2                (1UL<< 2)
#define LED3                (1UL<< 3)
#define LED4                (1UL<< 4)
#define LED5                (1UL<< 5)
#define LED6                (1UL<< 6)
#define LED7                (1UL<< 7)

#define LED_MASK            (LED0 | LED1 | LED2 | LED3 | LED4 | LED5 | LED6 | LED7)
#define LED_PORT            GPIO2_DR
#define LED_DIR             GPIO2_GDIR

#define LED0_ON()           (GPIO2_DR_bit.DR0 = 0)
#define LED0_OFF()          (GPIO2_DR_bit.DR0 = 1)
#define LED1_ON()           (GPIO2_DR_bit.DR1 = 0)
#define LED1_OFF()          (GPIO2_DR_bit.DR1 = 1)
#define LED2_ON()           (GPIO2_DR_bit.DR2 = 0)
#define LED2_OFF()          (GPIO2_DR_bit.DR2 = 1)
#define LED3_ON()           (GPIO2_DR_bit.DR3 = 0)
#define LED3_OFF()          (GPIO2_DR_bit.DR3 = 1)
#define LED4_ON()           (GPIO2_DR_bit.DR4 = 0)
#define LED4_OFF()          (GPIO2_DR_bit.DR4 = 1)
#define LED5_ON()           (GPIO2_DR_bit.DR5 = 0)
#define LED5_OFF()          (GPIO2_DR_bit.DR5 = 1)
#define LED6_ON()           (GPIO2_DR_bit.DR6 = 0)
#define LED6_OFF()          (GPIO2_DR_bit.DR6 = 1)
#define LED7_ON()           (GPIO2_DR_bit.DR7 = 0)
#define LED7_OFF()          (GPIO2_DR_bit.DR7 = 1)

#endif /* __BOARD_H */
