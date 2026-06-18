/******************************************************************************
Minimal code for setting up Timer0 to interrupt on compare match on channel 0
to interrupt at 100Hz
XTALFREQ 12000000         //XTAL frequency in Hz
PCLKFREQ (XTALFREQ/4)     //pclk must always be XTALFREQ/4?

Open terminal I/O window in debugger using View/Terminal I/O in C-SPY to see
VICVectAddr value in exception handler. This is not routed to the serial port
because UARTx is not configured and no implementation of putchar()

******************************************************************************/


#include <NXP/iolpc2148.h>
#include <stdio.h>
#include <intrinsics.h>

#define XTALFREQ 12000000         //XTAL frequency in Hz
#define PCLKFREQ (XTALFREQ/4)     //pclk must always be XTALFREQ/4?
#define TICKS_PER_SECOND 100      // TIMER0 interrupt is 100Hz

#define FALSE 0
#define TRUE !(FALSE)

void feed (void);

int main(void);
__irq __arm void IRQ_ISR_Handler (void);
void MM_TIMER0_ISR();

unsigned int bl_TimerFlag;

int main(void)
{
  bl_TimerFlag = 0;  // No background tasks performed yet

  /* Init MCU Clock */
  /* Fosc = 12MHz, Fpckl = 12MHz */
  PLLCON_bit.PLLC = PLLCON_bit.PLLE = 0; // Disable and disconnect PLL
  feed();                                // PLL feed sequence

  /* Init Peripherial divider Pckl = Clk/4 */
  VPBDIV_bit.VPBDIV = 0;

  T0IR=0xFF;           // reset match and capture event interrupts
  T0TC=0;              // Clear timer counter
  T0PR= 0;             // No Prescalar
  T0MR0=PCLKFREQ/100;  // Count up to 36,864 for 100Hz interrupt, period = 10ms
  T0MCR = 3;           // Reset Timer Counter & Interrupt on match
  T0TCR = 1;           // Counting enable

  VICIntSelect  =  0;             // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFF;     // Diasable all interrupts
  VICProtection = 0;              // VIC registers can be accessed in User or
                                  // privileged mode
  VICVectAddr = 0;                // Clear interrupt
  VICProtection = 0;              // Accesss VIC in USR | PROTECT


  VICIntSelect &= ~(1<<VIC_TIMER0);            // Timer 0 intrpt is an IRQ (VIC_TIMER0 = 4)
  VICVectAddr0 = (unsigned int)&MM_TIMER0_ISR; // Install ISR in VIC addr slot 0
  VICVectCntl0 = 0x20 | VIC_TIMER0;            // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER0);             // Turn on Timer0 Interrupt

  __enable_interrupt();                         // Global interrupt enable


  while(TRUE)                  // Foreground "task"
  {
    if(bl_TimerFlag)
    {
      bl_TimerFlag = FALSE;    // Clear this flag if set by MM_TIMER0_ISR
    }

  } // end foreground loop
}   // end main()

/******************************************************************************
 * Function Name: irq_handler
 * Parameters: void
 * Return: void
 *
 * Description: IRQ exception handler, this will call appropriate isr after
 * reading value out of VICVectAddr
* Note: This is ARM mode code - full 32 bit code	
 *****************************************************************************/
__irq __arm void irq_handler (void)
{
  void (*interrupt_function)();
  unsigned int vector;
  static unsigned int us_count;
  us_count++;
  vector = VICVectAddr;                   // Get interrupt vector.
  if(us_count == 100)
  {
    printf("VICVectAddr = %8x\n",vector); // print VICVectAddr @ 1Hz
    us_count = 0;
  }
  interrupt_function = (void(*)())vector; // Call MM_TIMER0_ISR thru pointer
  (*interrupt_function)();  // Call vectored interrupt function
  VICVectAddr = 0;          // Clear interrupt in VIC
}

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: void
 * Return: void
 *
 * Description: FIQ subroutine
* Note: This is ARM mode code - full 32 bit code	
 *************************************************************************/
__fiq __arm void fiq_handler (void)
{
  while(1);
}


void MM_TIMER0_ISR()
{
  static unsigned int us_Ticks;
  us_Ticks++;
  if(us_Ticks == TICKS_PER_SECOND)
  {
    bl_TimerFlag = TRUE;        // The background "task"
    us_Ticks = 0;
  }
  T0IR = 1;                     // Clear timer interrupt
}

// Feed sequence for the PLL
void feed (void)
{
  PLLFEED=0xAA;
  PLLFEED=0x55;
}
