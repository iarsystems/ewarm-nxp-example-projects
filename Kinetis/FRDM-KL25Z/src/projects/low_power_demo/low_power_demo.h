/******************************************************************************
* File:    low_power_demo.h
* Purpose: Declare functions used by the main application and define
* constants/macros.  
******************************************************************************/

#ifndef __LOW_POWER_DEMO_H
#define __LOW_POWER_DEMO_H 1

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/
void LowPowerModes_test(void);
void LLWU_Init(void);
void SW_LED_Init(void);
void demo_lptmr_isr(void);
void porta_isr(void);
void Port_Init(int io_nmi);
void LPTMR_init(int count, int clock_source);
void set_4_lp(unsigned char outof_lp);
void uart_configure(int mcg_clock_hz,int uart0_clk_src);
int PEE_to_BLPE(void);
int BLPE_to_PEE(void);
void clockMonitor(unsigned char state);
void JTAG_TDO_PullUp_Enable(void);
void vlp_clock_config(char next_mode);
void print_mcg_regs(void);
void systick_isr(void);
void enable_systick();


/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/
#define UNDEF_VALUE  0xFF
  #define LED0_EN (PORTA_PCR16 = PORT_PCR_MUX(1))
  #define LED1_EN (PORTA_PCR17 = PORT_PCR_MUX(1))
  #define LED2_EN (PORTB_PCR8 = PORT_PCR_MUX(1))
  #define LED3_EN (PORTA_PCR5 = PORT_PCR_MUX(1))

  #define LED0_TOGGLE (GPIOA_PTOR = (1<<16))
  #define LED1_TOGGLE (GPIOA_PTOR = (1<<17))
  #define LED2_TOGGLE (GPIOB_PTOR = (1<<8))
  #define LED3_TOGGLE (GPIOA_PTOR = (1<<5))

  #define LED0_OFF (GPIOA_PSOR = (1<<16))
  #define LED1_OFF (GPIOA_PSOR = (1<<17))
  #define LED2_OFF (GPIOB_PSOR = (1<<8))
  #define LED3_OFF (GPIOA_PSOR = (1<<5))

  #define LED0_ON (GPIOA_PCOR = (1<<16))
  #define LED1_ON (GPIOA_PCOR = (1<<17))
  #define LED2_ON (GPIOB_PCOR = (1<<8))
  #define LED3_ON (GPIOA_PCOR = (1<<5))
  /* fast GPIO through IOPORT */
  #define _LED0_TOGGLE (FGPIOA_PTOR = (1<<16))
  #define _LED1_TOGGLE (FGPIOA_PTOR = (1<<17))
  #define _LED2_TOGGLE (FGPIOB_PTOR = (1<<8))
  #define _LED3_TOGGLE (FGPIOA_PTOR = (1<<5))
  #define WAIT_TIME 0x0fff

#endif  //__LOW_POWER_DEMO_H

/* End of "low_power_demo.h" */
