/******************************************************************************
Example program to show how to send characters out of UART0 by polling. Char-
acters are sent out at rate of 1 every 10milliseconds. The data to be trans-
mitted is stored in a static string that wraps to beginning when the last char-
acter in the buffer is sent.

This example does not use the PLL to mulitply the main clock frequency, so the
main bus is running at 12MHz and the peripheral clock at 3MHz.

Notice that in this example, the statement VICVectAddr = 0; which is necessary
to signal the completion of an interrupt service routine, has been moved to
the __irq function. If you have a lot of interrupt sources, this saves redund-
ancy in code.

The foreground task transmits the characters every time the 50msec timer
expires. Use hyperterm or other terminal emulator with no flow control.

9 Jun 05
Usage note: if you do not see characters on hyperterm, halt the program by
clicking on the red hand, then restart.

10 Jun 05
Enabling FIFO buffers with U1FCR = 1; per Anders Lundgren's advice.

******************************************************************************/

#include "demo.h"

int main(void)
{
  SoftwareTimer_t *ptr2Timer = &softTimer[0];
  char *ptr2Str = &textString[0];

  initializeInterruptDataStructs();
  installTimer(ptr2Timer,FIFTY_mSEC_TIMER,TICKS_PER_50mSEC,  // install 50msec
               FIFTY_mSEC_TIMER_INSTALLED);                  // s'ware timer

  /* Init MCU Clock */
  /* Fosc = 12MHz, Fpckl = 12MHz */
  PLLCON_bit.PLLC = PLLCON_bit.PLLE = 0; // Disable and disconnect PLL
  feed();                                // PLL feed sequence

  /* Init Peripherial divider Pckl = Clk/4 */
  VPBDIV_bit.VPBDIV = 0;

  /* This section of code configures Timer 0, match channel 0 to interrupt on
  matching the value stored in MR0 */
  T0IR=0xFF;           // reset match and capture event interrupts
  T0TCR=0;             // Disable counting
  T0TC=0;              // Clear timer counter
  T0PR= 0;             // No Prescalar
  T0PC=0;              // Clear prescaler timer counter
  T0MR0=PCLKFREQ/100;  // Count up to 36,864 for 100Hz interrupt, period = 10ms
  T0MCR |= (RESET | INT_ON_MATCH);  // Reset Timer Counter & Interrupt on match
  T0TCR &= ~2;         // Clear reset flag
  T0TCR = 1;           // Counting enable

  /* Preliminary setup of the VIC. Assign all interrupt channels to IRQ */
  VICIntSelect  =  0;             // Set all VIC interrupts to IRQ for now
  VICIntEnClear = 0xFFFFFFFF;     // Diasable all interrupts
  VICSoftIntClear = 0xFFFFFFFF;   // Clear all software interrutps
  VICProtection = 0;              // VIC registers can be accessed in User or
                                  // privileged mode
  VICVectAddr = 0;                // Clear interrupt
  VICDefVectAddr = 0;             // Clear address of the default ISR

  /*Configure the pins that are connected to RX and TX on UART1 */
  PINSEL0 = (1<<ENABLE_UART1_RX) | (1<< ENABLE_UART1_TX); // PINSEL0 = 0x50000

  /*Configure UART1 to 9600 buad, 8 bit, 1 stop, no parity */
  U1FCR = 1;                      // Enable FIFOs whether used or not
  SetBit(U1LCR,DIVISOR_LATCH_SET_bit); // U1LCR = 0X80-enable access to divisor
                                  // latch bit, necessary for setting baud rate
  U1LCR |= EIGHT_BIT_CHARS;       // Eight bits U1LCR = 0X83
  ClrBit(U1LCR,PARITY_ENABLE_bit);     // No parity
  ClrBit(U1LCR,STOP_BIT_SELECT_bit);   // One stop bit
  U1DLL = PCLKFREQ / (9600 * 16);
  U1DLM = (PCLKFREQ / (9600 * 16)) >> 8;
  ClrBit(U1LCR,DIVISOR_LATCH_SET_bit); // Disable access to divisor latch

  /* Clear the VICVectAddress slots 0-15 and VICVectCtrl slots 0-15 */
  VICVectAddr0  = 0;
  VICVectAddr1  = 0;
  VICVectAddr2  = 0;
  VICVectAddr3  = 0;
  VICVectAddr4  = 0;
  VICVectAddr5  = 0;
  VICVectAddr6  = 0;
  VICVectAddr7  = 0;
  VICVectAddr8  = 0;
  VICVectAddr9  = 0;
  VICVectAddr10 = 0;
  VICVectAddr11 = 0;
  VICVectAddr12 = 0;
  VICVectAddr13 = 0;
  VICVectAddr14 = 0;
  VICVectAddr15 = 0;

                                  // Disable all vectored IRQ slots
  VICVectCntl0  = 0;
  VICVectCntl1  = 0;
  VICVectCntl2  = 0;
  VICVectCntl3  = 0;
  VICVectCntl4  = 0;
  VICVectCntl5  = 0;
  VICVectCntl6  = 0;
  VICVectCntl7  = 0;
  VICVectCntl8  = 0;
  VICVectCntl9  = 0;
  VICVectCntl10 = 0;
  VICVectCntl11 = 0;
  VICVectCntl12 = 0;
  VICVectCntl13 = 0;
  VICVectCntl14 = 0;
  VICVectCntl15 = 0;

  /* This section installs the specific interrupts and configure the VIC
  control registers and sets them as IRQs */
  VICProtection = 0;                           // Accesss VIC in USR | PROTECT
  VICDefVectAddr = (unsigned int)&NonVectISR;  // Install default ISR addr

  /* Setup the Timer0 interrupt on match interrupt */
  VICIntSelect &= ~(1<<VIC_TIMER0);            // Timer 0 intrpt is an IRQ
  VICVectAddr0 = (unsigned int)&MM_TIMER0_ISR; // Install ISR in VIC addr slot
  VICVectCntl0 = 0x20 | VIC_TIMER0;            // IRQ type, TIMER 0 int enabled
  VICIntEnable |= (1<<VIC_TIMER0);             // Turn on Timer0 Interrupt

  __enable_interrupt();                         // Global interrupt enable


  /*This is the foreground loop, which looks at data coming from the background
  loop. The user can insert own code to react to timer and button driven
  events */
  while(TRUE)                                      // Foreground Loop
  {
    if(softTimer[FIFTY_mSEC_TIMER].us_Event == TIMER_EXPIRED)
    {
        installTimer(ptr2Timer,FIFTY_mSEC_TIMER,TICKS_PER_50mSEC,
               FIFTY_mSEC_TIMER_INSTALLED);       // reinstall 50msec timer
      if(*ptr2Str == '\0')
      {
        sendByte(0xD);                            // carriage return;
        sendByte(0xA);                            // line feed
        ptr2Str = &textString[0];                 // wrap to start of string
      }
      sendByte(*ptr2Str++);                       // transmit next character
    }


  } // end foreground loop
}   // end main()

void installTimer
(
  SoftwareTimer_t *ptr2Timer,
  unsigned int us_offset,
  unsigned int us_ticks,
  unsigned int us_event
)
{
  (ptr2Timer+us_offset)->us_Ticks = us_ticks;
  (ptr2Timer+us_offset)->us_Event = us_event;
}

void initializeInterruptDataStructs(void)
{
  unsigned int i;
  for(i=0;i<MAX_SOFTWARE_TIMERS;i++)
  {
    softTimer[i].us_Ticks = 0;
    softTimer[i].us_Event = EVENT_UNDEFINED;
  }
}

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: void
 * Return: void
 *
 * Description: IRQ subroutine
* Note: This is ARM mode code - full 32 bit code	
 *************************************************************************/
__irq __arm void irq_handler (void)
{
  void (*interrupt_function)();
  unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function thru pointer
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

/*************************************************************************
 * Function Name: MM_TIMER0_ISR
 * Parameters: void
 * Return: void
 *
 * Description: TIMER0 interrupt subroutine
 *
 *************************************************************************/
void MM_TIMER0_ISR()
{
  unsigned int IntType;
  unsigned int i;
  for(i=0; i<MAX_SOFTWARE_TIMERS; i++)
    if(softTimer[i].us_Ticks > 1)            // if timer installed decrement
      softTimer[i].us_Ticks--;
    else
      softTimer[i].us_Event = TIMER_EXPIRED; // us_Ticks == 1; timer expired

  IntType = (T0IR & 0xFF);                   // confirm interrupt type
  T0IR = (IntType & 0xFF);                   // Clear timer interrupt
}

void NonVectISR(void)
{
  while(TRUE);           // You should never get here, so spin in tight loop
                         // to signal something is amiss
}

void sendByte(char byte)
{
    while (!(U1LSR &0x20)); //U1THR THRE bit set when U1THR contains valid data
    U1THR = byte;
}

// Feed sequence for the PLL
void feed (void)
{
  PLLFEED=0xAA;
  PLLFEED=0x55;
}
