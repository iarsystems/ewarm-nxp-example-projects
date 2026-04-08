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

void my_time_delay()
{
  for(volatile int i = 0; i < 500000; ++i)
  {
    /* nothing */
  } 
}

int main()
{

  IOMUXC->SINGLE.PTB0 = 0x000011A2; //enable GPIO output for PTB2
  IOMUXC->SINGLE.PTB1 = 0x000011A2; //enable GPIO output for PTB3
  //Make the A5 wait forever now
  int mask = PIN(22)|PIN(23);
  while(1)
  {
    GPIO0->PCOR = mask; //corresponds with PTB0-PTB3

    //Delay
    my_time_delay();

    //Turn off LED's by driving 1
    GPIO0->PSOR = mask; //corresponds with PTB0-PTB3

    //Delay
    my_time_delay();  
  }
  return 0;
}
