/******************************************************************************
Example program to show how to trigger external interrupts by pressing buttons
1 and 1 on the IAR LPC2148 evaluation board.
See schematic for button connections:
BTN1 is hooked up to EINT2 on Port Pin P0.15
BTN2 is hooked up to EINT0 on Port Pin P0.16

This example does not use the PLL to mulitply the main clock frequency, so the
main bus is running at 12MHz and the peripheral clock at 3MHz.

The foreground task just resets timers and button states which are altered in
interrupt service routines. The user can use these to signal tasks to be
done in the foreground.

There are three software timer, which expire every 100 milliseconds, every
second, and every five seconds. Button presses are counted and data stored in
the pressCount field of struct buttonData.

Usage Notes:
Option run to main is disabled to save watchpoint.
Project\Linker option box "With runtime control modules" unchecked to save
watchpoint

- During debug:
- Click J-Link\Watchpoints
- Check box for Watchpoint 0
- Enter "VICVectAddr" (case sensitive) int Address
- Enter 0xFFFFFFFF into Address mask
- Select radio buttons R/W and Word in Access Type and Data respecively
- In Data value enter 0x40000591 (ISR address of External Int 2)

Similarly for Watchpoint 1, all same settings except enter 0x400005C1 for Data
value

Click Run
- when you press button 1, you will break in function
__irq __arm void IRQ_ISR_Handler (void) and trace into void Btn1DownISR(void)
- similarly, when you press button 2 you will break at the same place and
trace into void Btn2DownISR(void)

******************************************************************************/

#include "demo.h"

int main(void)
{
  SoftwareTimer_t *ptr2Timer = &softTimer[0];
  unsigned int i;

  initializeInterruptDataStructs();
  installTimer(ptr2Timer,FIVE_SEC_TIMER,TICKS_PER_5_SECONDS,
               FIVE_SEC_TIMER_INSTALLED);
  installTimer(ptr2Timer,ONE_SEC_TIMER,TICKS_PER_SECOND,
               ONE_SEC_TIMER_INSTALLED);
  installTimer(ptr2Timer,ONE_HUNDRED_mSEC_TIMER,TICKS_PER_100mSEC,
               ONE_HUNDRED_mSEC_TIMER_INSTALLED);

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

  /*Configure the pins that the buttons are hooked up to to be external
  interrupts */
  PINSEL0_bit.P0_15=0x2;          // Set Port pin P0.15 function to EINT2
  PINSEL1_bit.P0_16=0x1;          // Set Port pin P0.16 function to EINT0

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

  /* Set up the button 1 pressed interrupt on EINT0 */
  VICIntSelect &= ~(1<<VIC_EINT0);              // IRQ on external int 0.
  VICVectAddr1 = (unsigned int)&Btn2DownISR;    // Install ISR in VIC addr slot
  VICVectCntl1 = 0x20 | VIC_EINT0;              // Enable vec int for EINT 0.
  VICIntEnable |= (1<<VIC_EINT0);               // Enable EINT0 interrupt.

  /* Set up the button 2 pressed interrupt on EINT2 */
  VICIntSelect &= ~(1<<VIC_EINT2);              // IRQ on external int 2.
  VICVectAddr2 = (unsigned int)&Btn1DownISR;    // Install EINT2 in VIC addr slot
  VICVectCntl2 = 0x20 | VIC_EINT2;              // Enable vect int for EINT2.
  VICIntEnable |= (1<<VIC_EINT2);               // Enable EINT 2 interrupt.

  __enable_interrupt();                         // Global interrupt enable


  /*This is the foreground loop, which looks at data coming from the background
  loop. The user can insert own code to react to timer and button driven
  events */
  while(TRUE)                                      // Foreground Loop
  {
    for(i = 1; i<MAX_BUTTONS; i++)
    {
      if(buttonData[i].us_buttonState == DOWN)
      {
        buttonData[i].us_buttonState = UP;
        buttonData[i].us_pressCount = 0;
      }
    }
    if(softTimer[FIVE_SEC_TIMER].us_Event == TIMER_EXPIRED)
        installTimer(ptr2Timer,FIVE_SEC_TIMER,TICKS_PER_5_SECONDS,
               FIVE_SEC_TIMER_INSTALLED);
    if(softTimer[ONE_SEC_TIMER].us_Event == TIMER_EXPIRED)
      installTimer(ptr2Timer,ONE_SEC_TIMER,TICKS_PER_SECOND,
               ONE_SEC_TIMER_INSTALLED);
    if(softTimer[ONE_HUNDRED_mSEC_TIMER].us_Event == TIMER_EXPIRED)
      installTimer(ptr2Timer,ONE_HUNDRED_mSEC_TIMER,TICKS_PER_100mSEC,
               ONE_HUNDRED_mSEC_TIMER_INSTALLED);
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
  for(i=0;i<MAX_BUTTONS;i++)
  {
    buttonData[i].us_buttonState = UP;
    buttonData[i].us_pressCount = 0;
  }
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
    if(softTimer[i].us_Ticks > 1)
      softTimer[i].us_Ticks--;
    else
      softTimer[i].us_Event = TIMER_EXPIRED;

  IntType = (T0IR & 0xFF); // confirm interrupt type
  T0IR = (IntType & 0xFF); // Clear timer interrupt
}


void Btn1DownISR(void)
{
  EXTINT_bit.EINT2 = 1;   // Try to reset external interrupt flag.
  if(!EXTINT_bit.EINT2)   // Check if flag was reset (button not pressed).
  {
    buttonData[1].us_buttonState = DOWN;
    buttonData[1].us_pressCount++;
  }
}

void Btn2DownISR(void)
{
  EXTINT_bit.EINT0 = 1;   // Try to reset external interrupt flag.
  if(!EXTINT_bit.EINT0)   // Check if flag was reset (button not pressed).
  {
    buttonData[2].us_buttonState = DOWN;
    buttonData[2].us_pressCount++;
  }
}

void NonVectISR(void)
{
  while(TRUE);           // You should never get here, so spin in tight loop
                         // to signal something is amiss
}

// Feed sequence for the PLL
void feed (void)
{
  PLLFEED=0xAA;
  PLLFEED=0x55;
}
