/*
 * File:        faraday_evb.h
 * Purpose:     Tower board definitions
 *
 * Notes:
 */

#ifndef __FARADAY_EVB_H__
#define __FARADAY_EVB_H__

/********************************************************************/

/* Global defines to use for all Firebird daughter cards */
#define DEBUG_PRINT

/* Define CPU version on the board */
#ifdef NO_PLL_INIT
  #define A5_CORE_CLK_KHZ     24000
  #define M4_CORE_CLK_KHZ     12000
  #define BUS_CLK_KHZ         12000
#else
  #define A5_CORE_CLK_KHZ     396000
  #define M4_CORE_CLK_KHZ     132000
  #define BUS_CLK_KHZ         66000
#endif

  /* Serial Port Info */
  #define UART_PORT           UART0
  #define TERMINAL_BAUD       115200
/* PTD16 (GPIO2[14] connector P17-1) */
#define LED0_GPIO 78

#define ENABLE_LEDS  	IOMUXC->RGPIO[LED0_GPIO] = 0x000011A2; //enable GPIO output

#define LED0_ON     { GPIO2->PCOR =  PIN(14); }
#define LED0_OFF    { GPIO2->PDOR =  PIN(14); }
#define LEDS_TOGGLE	{ GPIO2->PTOR |= PIN(14); }

#define LEDS_ON	    { LED0_ON;  }
#define LEDS_OFF    { LED0_OFF; }

/* Define push-buttons on Faraday EVB board */
#define PUSH_BUTTON0_GPIO	63
#define PUSH_BUTTON1_GPIO	64
#define PUSH_BUTTON2_GPIO	65
#define PUSH_BUTTON3_GPIO	97

/* enable GPIO input for PTD31, PORT1[31] (SW6), set IBE, enable pullup */
#define PUSH_BUTTON0_INIT 	{ IOMUXC->RGPIO[PUSH_BUTTON0_GPIO] = 0x0000000D;  }
/* enable GPIO input for PTD30, PORT2[0] (SW7), set IBE, enable pullup */
#define PUSH_BUTTON1_INIT 	{ IOMUXC->RGPIO[PUSH_BUTTON1_GPIO] = 0x0000000D; }
/* enable GPIO input for PTD29, PORT2[1] (SW8), set IBE, enable pullup */
#define PUSH_BUTTON2_INIT 	{ IOMUXC->RGPIO[PUSH_BUTTON0_GPIO] = 0x0000000D;  }
/* enable GPIO input for PTB27, PORT3[1] (SW9), set IBE, enable pullup */
#define PUSH_BUTTON3_INIT 	{ IOMUXC->RGPIO[PUSH_BUTTON0_GPIO] = 0x0000000D;  }

#define PUSH_BUTTONS_INIT	{ PUSH_BUTTON0_INIT; PUSH_BUTTON1_INIT; \
    PUSH_BUTTON2_INIT; PUSH_BUTTON3_INIT; }

#endif /* __FARADAY_EVB_H__ */
