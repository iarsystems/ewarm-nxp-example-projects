/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 16 $
 **
 ***************************************************************************/
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
#define SPLL_CLK            300MHZ

/* GPT1 Tick per second*/
#define GPT1_TICK_PER_SEC   100

/* CPLD */
#define CPLD_VER            (*(unsigned short *)0xD4000000)
#define BOAR_REV()          ( CPLD_VER    &0xFF)
#define CPLD_REV()          ((CPLD_VER>>8)&0xFF)

#define BCTRL1_SET          (*(unsigned short *)0xD4000008)
#define BCTRL1_CLR          (*(unsigned short *)0xD400000C)

#define LED1                (1UL<< 5)
#define LED2                (1UL<< 6)
#define LCDCTRL             (1UL<<11)

#define LED1_ON()           (BCTRL1_SET = LED1)
#define LED1_OFF()          (BCTRL1_CLR = LED1)

#define LED2_ON()           (BCTRL1_SET = LED2)
#define LED2_OFF()          (BCTRL1_CLR = LED2)

#define LCD_ON()            (BCTRL1_SET = LCDCTRL)
#define LCD_OFF()           (BCTRL1_SET = LCDCTRL)

#define BCTRL2_SET          (*(unsigned short *)0xD4000010)
#define BCTRL2_CLR          (*(unsigned short *)0xD4000014)

#define CSI_PWR             (1UL<< 9)
#define CSI_RST             (1UL<<10)

#define CSI_RST_H()         (BCTRL2_SET = CSI_RST)
#define CSI_RST_L()         (BCTRL2_CLR = CSI_RST)

#define CSI_STB_H()         (BCTRL2_SET = CSI_PWR)
#define CSI_STB_L()         (BCTRL2_CLR = CSI_PWR)

#define BCTRL4_SET          (*(unsigned short *)0xD4000020)
#define BCTRL4_CLR          (*(unsigned short *)0xD4000024)

#define USER_OFF            (1UL<< 1)

#define ATLAS_PWR_OFF()     (BCTRL4_SET = USER_OFF)
#define ATLAS_PWR_ON()      (BCTRL4_CLR = USER_OFF)

#define CSI_RST_H()         (BCTRL2_SET = CSI_RST)

#define BSTAT1              (*(unsigned short *)0xD4000028)

#define ATLAS_PRI_INT       (1UL<< 0)

#define ATLAS_INTR()        ((BSTAT1 & ATLAS_PRI_INT) != 0)

#endif /* __BOARD_H */
