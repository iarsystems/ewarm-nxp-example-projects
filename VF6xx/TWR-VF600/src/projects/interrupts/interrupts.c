#include <intrinsics.h>
#include "common.h"
#include "sysinit.h"
#include "interrupts.h"
#include "lpt.h"

int LPTMR_INTERRUPT;

void lptmr_isr_example(void)
{
  LPTMR_INTERRUPT=1;
  LPTMR0->CSR=0;   //Clear LPT Compare flag

  //Set global variable
  printf("\n\nIn LPT ISR!\n\n");
}

int main()
{
  //Enable interrupt for LPTimer (Vector 72 on A5)
  enable_interrupt(72,lptmr_isr_example);

  // Enable the IRQ, FIQ and Aborts in A5 Core
  __enable_interrupt();

  //Now setup the LPTimer to create an interrupt after 1 second

  LPTMR_INTERRUPT=0;  //Global flag variable

  /* Make sure the clock to the LPTMR is enabled */
  CCM->CCGR2 |= CCM_CCGR2_CG0(1);

  /* Reset LPTMR settings */
  LPTMR0->CSR=0;

  /* Set the compare value to the number of ms to delay */
  LPTMR0->CMR = 1000;

  /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
  LPTMR0->PSR |= LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK;

  /* Start the timer */
  LPTMR0->CSR |= LPTMR_CSR_TIE_MASK | LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value and set TCF flag*/
  while(LPTMR_INTERRUPT==0)
  {}

  /* Disable counter and Clear Timer Compare Flag */
  LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;


  led_blink_tower();

  return 0;
}

/* Blink LEDs on TWR-VF600 */
void led_blink_tower()
{
  //Setup GPIO output on LED pins (PTB0-PTB3)
  IOMUXC->SINGLE.PTB0 = IOMUXC_PTB0_DSE(0x1)| IOMUXC_PTB0_OBE_MASK; //enable GPIO output for PTB0
  IOMUXC->SINGLE.PTB1 = IOMUXC_PTB1_DSE(0x1)| IOMUXC_PTB1_OBE_MASK; //enable GPIO output for PTB1 
  IOMUXC->SINGLE.PTB2 = IOMUXC_PTB2_DSE(0x1)| IOMUXC_PTB2_OBE_MASK; //enable GPIO output for PTB2 
  IOMUXC->SINGLE.PTB3 = IOMUXC_PTB3_DSE(0x1)| IOMUXC_PTB3_OBE_MASK; //enable GPIO output for PTB3 


  //Flash on and off LED's
  while(1)
  {
    //Turn on LED's by driving 0 (active low)
    GPIO0->PDOR=~(PIN(22)|PIN(23)|PIN(24)|PIN(25)); //corresponds with PTB0-PTB3

    //Delay
    time_delay_ms(1000);

    //Turn off LED's by driving 1
    GPIO0->PDOR=PIN(22)|PIN(23)|PIN(24)|PIN(25); //corresponds with PTB0-PTB3

    //Delay
    time_delay_ms(1000);
  }
}
