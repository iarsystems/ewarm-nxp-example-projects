#include "common.h"
#include "sysinit.h"
#include "dual_core.h"
#include "lpt.h"

/*
 * Please read the readme.docx in \src\projects\dual_core for details
 *  about this demo
 */

void my_time_delay()
{
  for(volatile int i = 0; i < 500000; ++i)
  {
    /* nothing */
  } 
}

int do_blink()
{	
  IOMUXC->SINGLE.PTB2 = 0x000011A2; //enable GPIO output for PTB2
  IOMUXC->SINGLE.PTB3 = 0x000011A2; //enable GPIO output for PTB3
  //Make the A5 wait forever now
  while(1)
  {
    GPIO0->PCOR = PIN(24)|PIN(25); //corresponds with PTB0-PTB3

    //Delay
    my_time_delay();

    //Turn off LED's by driving 1
    GPIO0->PSOR = PIN(24)|PIN(25); //corresponds with PTB0-PTB3

    //Delay
    my_time_delay();  
  }
}

int main()
{
  int count = 0;
  printf("A5 Main!\n");

  printf("Starting M4 core\n");
  //Set starting point for M4 code
  SRC->GPR[2] = (unsigned int)0x3f040401; //must be odd address since M4 is thumb
  //Enable M4 core 
  CCM->CCOWR = 0x15a5a;
	
  do_blink();
}



