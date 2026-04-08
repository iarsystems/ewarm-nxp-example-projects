#ifndef __PLATFORM_PERIPHERALS_H
#define __PLATFORM_PERIPHERALS_H

//#pragma diag_suppress 368  // Suppresses warning from FreeRTOS code task.h file

#include <stdint.h>
#include "board.h"

#define RUN_FROM_RAM  //Define if code is to be executed from RAM!

void platform_uart_init(void);



/**
 * Initialize system clocks
 *
 * PORTME:  This function provides target specific system clocks initialization
 * needed by the application.  Since different build environments, e.g.,
 * Keil, IAR, LPCXpresso may perform different system startup clock
 * initializations we assume nothing but instead initialization everything to a
 * known state in this function.
 * 
 * For the system clock this function must:
 *  Initialize PLL and set it as the system clock with desired frequency
 * 
 * Typically this function would either directly reference CMSIS style registers 
 * or call library functions, e.g., LPCOpen functions, ROM APIs, xSW, etc.
 * 
 * If dual-core (MULTITHREAD == 2) the MASTER will setup the clock for both 
 * itself and the slave
 *
 */
void platform_clocks_init(void);



/**
 * Send a character to the system UART
 * 
 * PORTME:  This function provides a target specific UART send routine.  
 * Also see init_UART()
 * 
 * @param c  Character to send
 */
void platform_uart_send_byte(uint8_t c);

void platform_power_init(void);



#endif  // __PLATFORM_PERIPHERALS_H

