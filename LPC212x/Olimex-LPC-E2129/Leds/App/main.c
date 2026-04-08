/*************************************************************************
 *
 *    This program is created for LPC-E2129 board
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : June, 14 2006
 *       Author      : Todor Atanasov
 *
 *       Description : The initial purpose of this example is to show some of
 *       the timers functionality and working with the Vectored interrupt
 *       controller (VIC).
 *       The two buttons B1 and B2 of the LPC-E2129 evaluation board are
 *       connected to the EXT Interrupts. So on button press or release an
 *       interrupt is generated. The Buttons interrupt handles then starts
 *       a debounce delay by using the Match Registers. As an indication
 *       LED1 and LED2 are turned ON on pressing respectively B1 and B2.
 *       Timer1 interrupt is used to blink LED3. Blink is activated on pressing
 *       Button1 and stops on pressing Button2.
 *
 *      Jumpers:
        DBG  - must be filled
        BSL  - removed
        JRST - removed
 *
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "includes.h"
#include "led.h"
#include "buttons.h"

// Time Precision time [us]
#define TIMER_PRECISION     1
// Sys timer tick per seconds
#define TICK_PER_SECOND     20

#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;

LPC_SysControl_MAMConfig_t MamConfig = {MAM_CONFIG,MAM_CYCLES};

void TMR0_CH0 (void*);
void TMR0_CH1 (void*);
void TMR1_CH0_ISR ();

/*************************************************************************
*
*************************************************************************/
Int32U TickSysFlag;
Int8U Button1_EXTINT, Button2_EXTINT, TempPort;
Int8U Button1Status, Button2Status;
/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void IRQ_Handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();   // Call vectored interrupt function.
  }
  else
  {
    VICVectAddr = 0;        // Clear interrupt in VIC.
  }
}
/*************************************************************************
 * Function Name: EINT2_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler - external interrupts from Button (B1)
 *
 *************************************************************************/
void EINT2_ISR ()
{
    TIMER_SetMatchAction(TIMER0, CH0, TimerAction_Interrupt | TimerAction_ResetTimer,
    1sec_T0/TICK_PER_SECOND, TMR0_CH0, (void *)&TickSysFlag, DONOTHING);
    TIMER_Start(TIMER0);
    EXTINT_bit.EINT2 = 1;
    TIMER_SetMatchAction(TIMER1, CH0, TimerAction_Interrupt | TimerAction_ResetTimer,
    1sec_T0/TICK_PER_SECOND, TMR1_CH0_ISR, (void *)&TickSysFlag, DONOTHING);
    TIMER_Start(TIMER1);
    VICVectAddr = 0;        // Clear interrupt in VIC.
}
/*************************************************************************
 * Function Name: EINT3_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler - external interrupts from Button (B2)
 *
 *************************************************************************/
void EINT3_ISR ()
{
    TIMER_SetMatchAction(TIMER0, CH1, TimerAction_Interrupt | TimerAction_ResetTimer,
    1sec_T0/TICK_PER_SECOND, TMR0_CH1, (void *)&TickSysFlag, DONOTHING);
    TIMER_Start(TIMER0);
    EXTINT_bit.EINT3 = 1;
    TIMER_Stop(TIMER1);
    VICVectAddr = 0;        // Clear interrupt in VIC.
}
/*************************************************************************
 * Function Name: TMR1_CH0_ISR
 * Parameters: none
 *
 * Return: none
 *
 * Description: Toggle the LED3 state - ON/OFF on Timer1 Match
 *
 *************************************************************************/
void TMR1_CH0_ISR ()
{
    if(TempPort)
    {
        IO0CLR_bit.P0_11 = 1;   // LED3 ON
        TempPort = 0;
    }
    else
    {
        IO0SET_bit.P0_11 = 1;   // LED3 OFF
        TempPort = 1;
    }
}
/*************************************************************************
 * Function Name:  TMR0_CH0
 * Parameters: void *arg
 *
 * Return: none
 *
 * Description: Interrupt handler on Timer0 Match. Used for debounce (Button 1).
 *
 *************************************************************************/
void TMR0_CH0 (void* arg)
{
    TIMER_Stop(TIMER0);
    PINSEL0_bit.P0_15 = 0;      // B1 - as GPIO port
    if(Button1_EXTINT == ON_KEY_PRESSED)
    {
        if(BUT1_STATE == PRESSED)
        {
            // Set the External Interrupt Polarity bit for Button1. It is done this way because of a problem.
            // Please refer to ERRATA SHEET(LPC2129,LPC2129/00) from 17, May, 2006
            APBDIV_bit.APBDIV = 0;
            EXTPOLAR_bit.EXTPOLAR2 = 1;     // EINT2 is rising-edge sensitive
            APBDIV_bit.APBDIV = 0;
            APBDIV_bit.APBDIV = VPBDIV1;
            //
            LED1_ON();
            Button1_EXTINT = ON_KEY_RELEASED;
        }
    }
    else
    {
        if(BUT1_STATE == RELEASED)
        {
            // Set the Eternal Interrupt Polarity bit for Button2.
            APBDIV_bit.APBDIV = 0;
            EXTPOLAR_bit.EXTPOLAR2 = 0;     // EINT2 is falling-edge sensitive
            APBDIV_bit.APBDIV =   0;
            APBDIV_bit.APBDIV = VPBDIV1;
            //
            LED1_OFF();
            Button1_EXTINT = ON_KEY_PRESSED;
        }
    }
    PINSEL0_bit.P0_15 = 2;              // EINT2
}
/*************************************************************************
 * Function Name: TMR0_CH1
 * Parameters: void *arg
 *
 * Return: none
 *
 * Description: Interrupt handler on Timer0 Match. Used for debounce Button 2.
 *
 *************************************************************************/
void TMR0_CH1 (void* arg)
{
    TIMER_Stop(TIMER0);
    PINSEL0_bit.P0_9 = 0;         // B2 - as GPIO port
    if(Button2_EXTINT == ON_KEY_PRESSED)
    {
        if(BUT2_STATE == PRESSED)
        {
            APBDIV_bit.APBDIV= 0;
            EXTPOLAR_bit.EXTPOLAR3 = 1;     // EINT3 is rising-edge sensitive
            APBDIV_bit.APBDIV =   0;
            APBDIV_bit.APBDIV = VPBDIV1;
            LED2_ON();
            Button2_EXTINT = ON_KEY_RELEASED;
        }
    }
    else
    {
        if(BUT2_STATE == RELEASED)
        {
            APBDIV_bit.APBDIV = 0;
            EXTPOLAR_bit.EXTPOLAR3 = 0;     // EINT3 is falling-edge sensitive
            APBDIV_bit.APBDIV =   0;
            APBDIV_bit.APBDIV = VPBDIV1;
            LED2_OFF();
            Button2_EXTINT = ON_KEY_PRESSED;
        }
    }
    PINSEL0_bit.P0_9 = 3;         // EINT3
}
// ************************************************************************

int main()
{
  /***** INITIALIZATION *****/

  SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, MAM_STATE, &MamConfig, 0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0);
  // Led initialization
  LedsInit();
  // Buttons initialization
  InitButtons();
  Button1_EXTINT = ON_KEY_PRESSED;
  Button2_EXTINT = ON_KEY_PRESSED;

  VIC_Init();
  VIC_SetProtectionMode(UserandPrivilegedMode);

  // Init System Timer - Timer0
  TIMER_Init(TIMER0, TIMER_PRECISION);

  // System Timer interrupt registered
  VIC_SetVectoredIRQ(TIMER0_ISR,VIC_Slot2,VIC_TIMER0);
  VIC_EnableInt(1<<VIC_TIMER0);

  // External interrupt enable
  VIC_SetVectoredIRQ(EINT2_ISR,VIC_Slot4,VIC_EINT2);
  VIC_EnableInt(1<<VIC_EINT2);

  VIC_SetVectoredIRQ(EINT3_ISR,VIC_Slot5,VIC_EINT3);
  VIC_EnableInt(1<<VIC_EINT3);

  // Init Timer1
  TIMER_Init(TIMER1, TIMER_PRECISION);
  // interrupt registered
  VIC_SetVectoredIRQ(TIMER1_ISR,VIC_Slot3,VIC_TIMER1);
  VIC_EnableInt(1<<VIC_TIMER1);

  __enable_interrupt();
//-----------------------------------------------------------------------------

  // loop forever
  while(1) {
        ;
  }

}
