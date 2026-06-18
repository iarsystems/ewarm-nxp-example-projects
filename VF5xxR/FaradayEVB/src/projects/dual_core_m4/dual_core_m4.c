#include "common.h"
#include "sysinit.h"
#include "dual_core_m4.h"
#include "lpt.h"

extern void enable_irq(int irq);
extern void write_vtor(int vtor);

volatile int LPTMR_INTERRUPT;

void lptmr_isr_example(void)
{
  LPTMR_INTERRUPT=1;
  LPTMR0->CSR=0;   //Clear LPT Compare flag

  //Set global variable
  printf("\n\nIn LPT ISR!\n\n");
}

int main()
{
  printf("Hello from M4!\n");

  //Direct interrupt to core_1 (m4)
  MSCM->IRSPRC[40] |= MSCM_IRSPRC_CP1En_MASK;  //interrupt ID

  //Enable Interrupt
  enable_irq(40);


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

  __enable_irq();

  /* Wait for counter to reach compare value and set TCF flag*/
  while(LPTMR_INTERRUPT==0)
  {}

  /* Disable counter and Clear Timer Compare Flag */
  LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;


  led_blink();
}

/* Blink LEDs */
void led_blink()
{
  ENABLE_LEDS;

  //Setup GPIO output on LED pins (PTB0-PTB3)
  IOMUXC->SINGLE.PTB0 = 0x000011A2; //enable GPIO output for PTB0
  IOMUXC->SINGLE.PTB1 = 0x000011A2; //enable GPIO output for PTB1
  IOMUXC->SINGLE.PTB2 = 0x000011A2; //enable GPIO output for PTB2
  IOMUXC->SINGLE.PTB3 = 0x000011A2; //enable GPIO output for PTB3

  //Flash on and off LED
  while(1)
  {
    //Turn on LED's by driving 0 (active low)
    GPIO0->PDOR=~(PIN(22)|PIN(23)|PIN(24)|PIN(25)); //corresponds with PTB0-PTB3

    LEDS_TOGGLE;

    //Delay
    time_delay_ms(1000);

    //Turn off LED's by driving 1
    GPIO0->PDOR=PIN(22)|PIN(23)|PIN(24)|PIN(25); //corresponds with PTB0-PTB3

    //Delay
    time_delay_ms(1000);
  }
}
