/*
 * File:        freedom.h
 * Purpose:     Kinetis Freedom hardware definitions
 *
 * Notes:
 */

#ifndef __FREEDOM_H__
#define __FREEDOM_H__

#include "mcg.h"

/********************************************************************/

/* Global defines */
#define DEBUG_PRINT

/* Define the CPU used on this hardware platform */
#define CPU_MKL25Z128LK4

/* Define the clock parameters on this hardware platform */
#define CLK0_FREQ_HZ        8000000
#define CLK0_FREQ_MHZ		8
#define CLK0_TYPE           CRYSTAL
#undef	NO_PLL_INIT
#define PLL0_PRDIV      	(CLK0_FREQ_MHZ/2)
#define PLL0_VDIV       	24

/* Define the default serial port interface */
#define TERM_PORT_NUM       0
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL

#endif /* __FREEDOM_H__ */
