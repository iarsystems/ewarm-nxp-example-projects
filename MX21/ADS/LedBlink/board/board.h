/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 16 $
 **
 ***************************************************************************/
#ifndef __BOARD_H
#define __BOARD_H

#include "arm_comm.h"
/* OSC [MHz] */
#define FOSC1               (26MHZ)
#define FOSC2               (327680HZ)

/* Core clk [MHz]  266 MHz */
#define MPLL_CLK            (266MHZ)
#define FCLK                (266MHZ)
#define HCLK                (133MHZ)
// Peripheral IPCLK
#define IPCLK               (HCLK/2UL)

// Peripherals divider 1
#define PERDIV1             (16UL)
// Peripherals divider 2
#define PERDIV2             (16UL)
// Peripherals divider 4
#define PERDIV4             (5UL)

/* GPT1 Tick per second*/
#define GPT1_TICK_PER_SEC   (100UL)

// SDRAM definitions
#define SDRAM_BASE_ADDRESS  0xC0000000

// signal A10 of SDRAM into 16M x 16 x 2 mode IAM=0
#define SDRAM_PRECHARGE_ALL (Int32U)(1<<21)

// SDRAM Mode Register
// Burst Length = 8, Burst Typ - Sequential,
// Latency Mode - 3, Write Mode - Burst Writes
#define SDRAM_MODE_REG      0x00119800

// Burst Flash definitions
#define FLASH_BASE_ADDRESS  0xC8000000

// Boards revision base address
#define REV_BASE_ADDRESS    0xCC400000

// DBG UART (UART1) definitions
#define UART1_BAUD_RATE     115200

// 0xCC800000 definition
#define HD_CTRL_BASE_ADD    (volatile unsigned short *)0xCC800000
#define HD_CTRL_WR(Val)     (*HD_CTRL_BASE_ADD = Val)
#define HD_CTRL_RD(Val)     (Val = *HD_CTRL_BASE_ADD)

#define __TP6               0
#define __TP6_BIT           (1UL<<__TP6)
#define __TP7               1
#define __TP7_BIT           (1UL<<__TP7)
#define __RESET_E_UART      2
#define __RESET_E_UART_BIT  (1UL<<__RESET_E_UART)
#define __RESET_BASE        3
#define __RESET_BASE_BIT    (1UL<<__RESET_BASE)
#define __CSI_FLASH         4                       // THE IM8012'S FLASH PIN IS OUTPUT  !!!!
#define __CSI_FLASH_BIT     (1UL<<__CSI_FLASH)
#define __CSI_RST           5
#define __CSI_RST_BIT       (1UL<<__CSI_RST)
#define __CSI_STANDBY       6
#define __CSI_STANDBY_BIT   (1UL<<__CSI_STANDBY)
#define __UART1_EN          7
#define __UART1_EN_BIT      (1UL<<__UART1_EN)
#define __UART4_EN          8
#define __UART4_EN_BIT      (1UL<<__UART4_EN)
#define __LCDON             9
#define __LCDON_BIT         (1UL<<__LCDON)
#define __IRDA_EN           10
#define __IRDA_EN_BIT       (1UL<<__IRDA_EN)
#define __IRDA_FIR_SEL      11
#define __IRDA_FIR_SEL_BIT  (1UL<<__IRDA_FIR_SEL)
#define __IRDA_MD0_B        12
#define __IRDA_MD0_B_BIT    (1UL<<__IRDA_MD0_B)
#define __IRDA_MD1          13
#define __IRDA_MD1_BIT      (1UL<<__IRDA_MD1)
#define __LED4_ON           14
#define __LED4_ON_BIT       (1UL<<__LED4_ON)
#define __LED3_ON           15
#define __LED3_ON_BIT       (1UL<<__LED3_ON)

#define CSI_RST_L()         (*HD_CTRL_BASE_ADD &= ~__CSI_RST_BIT)
#define CSI_RST_H()         (*HD_CTRL_BASE_ADD |=  __CSI_RST_BIT)
#define CSI_STB_L()         (*HD_CTRL_BASE_ADD &= ~__CSI_STANDBY_BIT)
#define CSI_STB_H()         (*HD_CTRL_BASE_ADD |=  __CSI_STANDBY_BIT)

#define LCD_OFF()           (*HD_CTRL_BASE_ADD &= ~__LCDON_BIT)
#define LCD_ON()            (*HD_CTRL_BASE_ADD |=  __LCDON_BIT)

#define LED_STAT1_OFF()     (*HD_CTRL_BASE_ADD &= ~__LED4_ON_BIT)
#define LED_STAT1_ON()      (*HD_CTRL_BASE_ADD |=  __LED4_ON_BIT)

#define LED_STAT2_OFF()     (*HD_CTRL_BASE_ADD &= ~__LED3_ON_BIT)
#define LED_STAT2_ON()      (*HD_CTRL_BASE_ADD |=  __LED3_ON_BIT)

// LCD definitions
#define IMAGE_WIDTH         (240UL)
#define IMAGE_HEIGHT        (320UL)
#define IMAGE_BPP           (16UL)
#define LCD_WIDTH           (240UL)
#define LCD_HEIGHT          (320UL)
#define LCD_BPP             (16UL)

// CMOS Sensor definitions
#define CAM_SIZE_X          (640UL)
#define CAM_SIZE_Y          (480UL)
#define CAM_P_SIZE_X        (200UL)
#define CAM_P_SIZE_Y        (151UL)
#define CAM_BPP             (16UL)

#endif /* __BOARD_H */
