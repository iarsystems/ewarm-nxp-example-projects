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
  lptmr_pulse_counter(LPTMR_ALT1);
  lptmr_pulse_counter(LPTMR_ALT2);
  //lptmr_pulse_counter(LPTMR_ALT3); //LPMR_ALT3 not supported on TWR-K70 board
  
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
