/*
 * File:        lpt.c
 * Purpose:     Provide a simple way to delay for a set number of milliseconds
 *
 */


#include "common.h"
#include "lpt.h"

/********************************************************************/
/*
 * Initialize the low power timer to provide a delay measured in ms.
 *
 *
 * Parameters:
 *  count_val   number of ms to delay
 *
 * Returns:
 * None
 */
void time_delay_ms(uint32 count_val)
{
  /* Make sure the clock to the LPTMR is enabled */
  CCM->CCGR2 |= CCM_CCGR2_CG0(1);

  /* Reset LPTMR settings */
  LPTMR0->CSR=0;

  /* Set the compare value to the number of ms to delay */
  LPTMR0->CMR = count_val;

  /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
  LPTMR0->PSR |= LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;

  /* Start the timer */
  LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
  
  /* Wait for counter to reach compare value and set TCF flag*/
  while (!(LPTMR0->CSR & LPTMR_CSR_TCF_MASK));

  /* Disable counter and Clear Timer Compare Flag */
  LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
}
