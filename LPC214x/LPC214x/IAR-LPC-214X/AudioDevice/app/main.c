/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : June, 04 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 *  PWR_J   - depend of power source
 *  PWM/DAC - 2-3 (PWM)
 *  MIC/TR  - 1-2 (MIC)
 *  DBG     - present
 *
 * The example implements USB audio device with one output and one input.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define LCD_BACKLIGHT_ON()    (IO0SET_bit.P0_21 = 1)
#define LCD_BACKLIGHT_OFF()   (IO0CLR_bit.P0_21 = 1)
#define LCD_BACKLIGHT_INIT()  (PINSEL1_bit.P0_21 = 0, IO0DIR_bit.P0_21 = 1, IO0CLR_bit.P0_21 = 1)

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void irq_handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();  // Call vectored interrupt function.
  }
  else
  {
    VICVectAddr = 0;      // Clear interrupt in VIC.
  }
}

/*************************************************************************
 * Function Name: SetTickFlag
 * Parameters: none
 *
 * Return: none
 *
 * Description: Set arg
 *
 *************************************************************************/
void SetTickFlag (void* arg)
{
Int32U* pFlag = arg;
  *pFlag = 1;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 * Return: void
 *
 * Description: Timer1 CH0 subroutine - delay [100us]
 *		
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
volatile Int32U Flag = 0;
  // Stop Timer 1
  TIMER_Stop(TIMER1);
  // Reset Timer 1 counter
  TIMER_Reset(TIMER1);
  // Set action - match module CH0
  TIMER_SetMatchAction(TIMER1, CH0, TimerAction_Interrupt | TimerAction_StopTimer,
  Delay usec_T1*100, SetTickFlag, (void*)&Flag, DONOTHING);
  // Start Timer 1
  TIMER_Start(TIMER1);
  // Wait expire of delay
  while(Flag == 0);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main()
{
  SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, 0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0);
  VIC_Init();
  VIC_SetProtectionMode(UserandPrivilegedMode);
  // Init Dly 100 ms - Timer1
  PM_OpenPeripheral(PC_TIMER1);
  TIMER_Init(TIMER1, TIMER_PRECISION);
  //  Dly 100 ms interrupt registered
  VIC_SetVectoredIRQ(TIMER1_ISR,VIC_Slot2,VIC_TIMER1);
  VIC_EnableInt(1<<VIC_TIMER1);

  // Init Audio
  UsbAudioClassInit();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Enable interrupts
  __enable_interrupt();

  // Start System timer
  TIMER_Start(TIMER0);

  // LCD Powerup init
  HD44780_PowerUpInit();

  // Backlight On
  LCD_BACKLIGHT_INIT();
  LCD_BACKLIGHT_ON();

  // Show messages on LCD
  HD44780_StrShow(1, 1,  "  IAR Systems   ");
  HD44780_StrShow(1, 2,  "Audio Class Dev ");

  while(1)
  {
  }
}
