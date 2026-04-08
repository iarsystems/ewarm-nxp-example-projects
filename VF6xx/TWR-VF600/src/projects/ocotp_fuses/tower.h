/*
 * File:        tower.h
 * Purpose:     Tower board definitions
 *
 * Notes:
 */

#ifndef __TOWER_H__
#define __TOWER_H__

/********************************************************************/

/* Global defines to use for all Firebird daughter cards */
#define DEBUG_PRINT

/* Define CPU version on the board */
#if ( (defined(TOWER)) )

#ifdef NO_PLL_INIT
  #define A5_CORE_CLK_KHZ 24000
  #define M4_CORE_CLK_KHZ 12000
  #define BUS_CLK_KHZ     12000
#else
  #define A5_CORE_CLK_KHZ 396000
  #define M4_CORE_CLK_KHZ 132000
  #define BUS_CLK_KHZ      66000
#endif

  /* Serial Port Info */
  #define UART_PORT           UART1
  #define TERMINAL_BAUD       115200

#else
  #error "No valid board defined"
#endif


#endif /* __TOWER_H__ */
