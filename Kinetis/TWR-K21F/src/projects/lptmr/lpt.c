/*
 * File:		lptmr.c
 * Purpose:		Low Power Timer (LPTMR) examples
 *
 */

#include "lptmr.h"

/********************************************************************/
void main (void)
{
  printf("\n\n****************************\n");
  printf("LPTMR Examples\n");
  printf("****************************\n\n");

  //Enable Port clocks
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

  /* Enable LPT Module */
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;

  PORTB_PCR7 |= PORT_PCR_MUX(1);
  GPIOB_PDDR |= GPIO_PDDR_PDD(1 << 7); 
  
  /* Time counting using LPO clock */
  lptmr_time_counter();

  /* Time counting using LPO clock with Prescaling */
  lptmr_prescale();

  /* Time counting using different clock sources */
  lptmr_internal_ref_input();
  lptmr_lpo_input();
  lptmr_32khz_input();
  lptmr_external_clk_input();

  /* LPTMR Interrupt Example */
  lptmr_interrupt();

  /* Basic Pulse Counting */
  lptmr_pulse_counter(LPTMR_ALT1);  //LPMR_ALT1 not supported on TWR-K21D50M board
  lptmr_pulse_counter(LPTMR_ALT2);
  
  printf("\n****************************\n");
  printf("End of LPTMR Examples\n");
  printf("****************************\n");
}
/********************************************************************/

/*
 * Get the current LPTMR Counter Value. 
 *
 * On each read of the LPTMR counter register, software must first write to the 
 * LPTMR counter register with any value. This will synchronize and register the
 * current value of the LPTMR counter register into a temporary register. The 
 * contents of the temporary register are returned on each read of the LPTMR 
 * counter register.
 */
uint16 get_counter_value()
{
  LPTMR0_CNR=0x1;
  return LPTMR0_CNR;
}

/*
 * Zero out all registers.
 *
 */
void lptmr_clear_registers()
{
  LPTMR0_CSR=0x00;
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
}
