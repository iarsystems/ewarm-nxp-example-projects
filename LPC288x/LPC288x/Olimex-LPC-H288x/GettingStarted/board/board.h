/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/
#ifndef __BOARD_H
#define __BOARD_H

// OSCs [Hz]
#define MAIN_OSC_FREQ	       (12000000UL)
#define LF_OSC_FREQ		       (32768UL)

// PLL
#define SYS_FREQ    (12MHZ)

#if   SYS_FREQ == (60MHZ)
#define PLL_MUL               5
#define PLL_DIV               1
#elif SYS_FREQ == (36MHZ)
#define PLL_MUL               3
#define PLL_DIV               2
#elif SYS_FREQ == (12MHZ)
#define PLL_MUL               0
#define PLL_DIV               0
#else
#error SYS_FREQ must be 12MHZ, 36MHZ or 60MHZ
#endif

// SDRAM K4S561632L-TC75
// [ns]
#define SDRAM_BASE_ADDR       0x30000000

#if   SYS_FREQ == (60MHZ)
#define SDRAM_PERIOD          16.6  // 60MHz
#elif SYS_FREQ == (36MHZ)
#define SDRAM_PERIOD          27.8  // 36MHz
#elif SYS_FREQ == (12MHZ)
#define SDRAM_PERIOD          83.3  // 12MHz
#endif


#define P2C(Period)           (((Period<SDRAM_PERIOD)?0:(Int32U)((Flo32)Period/SDRAM_PERIOD))+1)

#define SDRAM_REFRESH         15625
#define SDRAM_TRP             20
#define SDRAM_TRAS            45
#define SDRAM_TAPR            1
#define SDRAM_TDAL            2
#define SDRAM_TWR             3
#define SDRAM_TRC             65
#define SDRAM_TRFC            66
#define SDRAM_TXSR            67
#define SDRAM_TRRD            15
#define SDRAM_TMRD            3

// 28F160C3BD70
#define FLASH_TAC             70

#endif /* __BOARD_H */
