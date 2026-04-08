
#ifndef _LOW_POWER_H
#define _LOW_POWER_H

#include "common.h"

/* Defines */
#define ENABLE_WAIT_MODE          0
#define ENABLE_STOP_MODE          1
#define ENABLE_LPSTOP1_MODE       2
#define ENABLE_LPSTOP2_MODE       3
#define ENABLE_LPSTOP3_MODE       4
#define ENABLE_LPRUN_MODE         5
#define ENABLE_ULPRUN_MODE        6

#define WAKE_FROM_INTERRUPT_TRUE    	0x1A
#define WAKE_FROM_INTERRUPT_FALSE   	0x1F
#define WAKE_FROM_INTERRUPT_DONT_CARE	0x1D

#define USE_INTERNAL_CLOCK				0x11
#define USE_EXTERNAL_CLOCK				0xEE

#define CLK_OFF_ALL_MODES               0x0
#define CLK_ON_RUN_OFF_WAIT_STOP_MODES  0x1
#define CLK_ON_ALL_MODES                0x2
#define CLK_ON_ALL_MODES_EXCEPT_STOP    0x3

/* defines for WKPU */
#define WKUP_SOURCE_14					  14	  // PTB27 (SW9)
#define RISING_EDGE_ENABLED				0x9E
#define FALLING_EDGE_ENABLED			0xFE

/* defines for lcd */
#define LCD_SYS_CLK           0
#define LCD_IRC_CLK           1
#define LCD_EXT_OSC           2

/* Switch Settings */
#define READ_FOR_SW1

/* Prototypes */
void low_power_mode_entry(uint32_t low_power_mode, uint32_t wake_from_interrupt, uint32_t clk_int_ext);
void print_pwr_mode(void);
void print_clk_mode(void);
void led_blink(void);
void read_potentiometer(void);
void low_power_setup(void);
void ptb_isr(void);
void wkpu_isr(void);
void enable_wkpu(uint32 source, uint32_t rise_fall);

#endif //#ifndef _LOW_POWER_H
