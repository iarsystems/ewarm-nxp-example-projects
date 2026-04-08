/******************************************************************************
Example program to show how to set up A/D converter on LPC2148. This example
sets up AD0 for a conversion every second of voltage going into channel 3.
This voltage is controlled by potentiometer on lower right hand corner of IAR
LPC2148 evaluation board.

The last A/D conversion is fetched in the foreground loop. A one second timer
in the background loop signals when the next conversion is to take place.
After the last converions is fetched, the next one is started for acquisition
the next time the one-sec timer times out.

Turn the blue potentiometer knob on the lower right hand corner of the eval-
uation board to see the the data change values.

11 Jun 05
Usage note: if you do not see characters on hyperterm, halt the program by
clicking on the red hand, then restart.


******************************************************************************/

#include "demo.h"
#include <stdio.h>
char str[100];
double ADCresmV;
int main(void)
{
  SoftwareTimer_t *ptr2Timer = &softTimer[0];
  unsigned int ADCresult;


  initializeInterruptDataStructs();       // Set up software timer data struct
  installTimer(                           // install 1 second software timer
               ptr2Timer,                 // that runs off master timer, which
               ONE_SEC_TIMER,             // is 100Hz Timer0 interrupt on com-
               TICKS_PER_1_SEC,           // pare to match registere 0
               ONE_SEC_TIMER_INSTALLED
               );


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


  /* Configure the Analog to Digital converter */

  AD0CR_bit.SEL = 8;      //enable AD0.3 only
  AD0CR_bit.CLKDIV = PCLKFREQ / 4500000;
  AD0CR_bit.BURST = 1;    // put A/D into continuous convert mode
  AD0CR_bit.CLKS = 0;     //11 clocks/10 bit accuracy
  AD0CR_bit.PDN = 1;      //power up the unit
  PINSEL1_bit.P0_30 = 1;  // This is probably the potentiometer output

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

  printf("potentiometer now reads %d\n\r",100);
  printf("potentiometer now reads %4.2f\n\r",100.0);
  sprintf(str,"potentiometer now reads %d\n\r",100);
  sprintf(str,"potentiometer now reads %4.2f\n\r",100.0);
  AD0CR_bit.START = 0x0001;                     //start 1st cnvrsn immediately

  /*This is the foreground loop, which looks at data coming from the background
  loop. The user can insert own code to react to timer and button driven
  events */

  while(TRUE)                                     // Foreground Loop
  {
    if(softTimer[ONE_SEC_TIMER].us_Event == TIMER_EXPIRED)
    {

      installTimer(                              // reinstall 1 sec
                   ptr2Timer,
                   ONE_SEC_TIMER,
                   TICKS_PER_1_SEC,
                   ONE_SEC_TIMER_INSTALLED
                  );

      /* Fetch the last A/D conversion and start up the A/D for the next one */
      while(AD0DR_bit.DONE == 0);                //wait until conversion done
      AD0DR_bit.CHN = 0;                         // Data in channel 0
      ADCresult = AD0DR_bit.VVDDA ;              // save the converted data
      ADCresmV = (ADCresult>>2)*3000.0/256;        // 10 bit resolution, 3V max
      printf("potentiometer now reads %4.2f\n\r",ADCresmV);
      AD0CR_bit.START = 0x0001;                  // start the next conversion
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

int putchar(int c)
{
   while(U1LSR_bit.THRE == 0); // wait until transmitter ready
   U1THR = c;                  //send the character
   return c;
}

// Feed sequence for the PLL
void feed (void)
{
  PLLFEED=0xAA;
  PLLFEED=0x55;
}
