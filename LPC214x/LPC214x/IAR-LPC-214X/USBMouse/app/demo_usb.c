/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name      : demo_usb.c
 *    Description    : Define main module
 *
 *    History :
 *    1. Date        : June 13, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Buttons function
 * Butt1 - Left movement of the mouse marker
 * Butt2 - Right movement of the mouse marker
 * Trimer function
 * Clock-wise         - Up movement of the mouse marker
 * Counter-clock-wise - Down movement of the mouse marker
 *
 * Jumpers settings
 * PWR_J - depend of voltage source
 * VREFF - present
 * DBG   - present
 *
 * Switches settings
 * ICSP 1,2 - Off
 *
 *    $Revision: 28 $
 *
**************************************************************************/
#include "includes.h"

#define LCD_BACKLIGHT_ON()    (IO0SET_bit.P0_21 = 1)
#define LCD_BACKLIGHT_OFF()   (IO0CLR_bit.P0_21 = 1)
#define LCD_BACKLIGHT_INIT()  (PINSEL1_bit.P0_21 = 0, IO0DIR_bit.P0_21 = 1, IO0CLR_bit.P0_21 = 1)
/* Define minimum changes of voltage that provoke movement of the mouse marker */
#define MIN_CHANGE 5

unsigned int AdcHold;

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: void
 * Return: void
 *
 * Description: IRQ subroutine
 *		
 *************************************************************************/
__irq __arm void irq_handler (void)
{
void (*interrupt_function)();
unsigned int vector;
  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  (*interrupt_function)();  // Call vectored interrupt function.
}

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 * Return: none
 *
 * Description: FIQ subroutine
 *
 *************************************************************************/
__fiq __arm void fiq_handler (void)
{
  while(1);
}

/*************************************************************************
 * Function Name: NonVectISR
 * Parameters: none
 * Return: none
 *
 * Description: non vectored callback subroutine
 *
 *************************************************************************/
void NonVectISR(void)
{
}

/*************************************************************************
 * Function Name: ClearFlag
 * Parameters: void* arg
 * Return: none
 *
 * Description: clear arg
 *
 *************************************************************************/
void ClearFlag (void* arg)
{
int * pFlag = (int *)arg;
  *pFlag = 0;
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
volatile int Flag = 1;
  // Stop Timer 1
  TIMER_Stop(TIMER1);
  // Stop Reset Timer 1 counter
  TIMER_Reset(TIMER1);
  // Set action of match module CH0
  TIMER_SetMatchAction(TIMER1, CH0, TimerAction_Interrupt | TimerAction_StopTimer,
  (Delay usec_T1)*100, ClearFlag, (void *)&Flag, DONOTHING);
  // Start Timer 1
  TIMER_Start(TIMER1);
  // Wait expire of delay
  while(Flag);
}

/*************************************************************************
 * Function Name: SysInit
 * Parameters: none
 * Return: int
 *
 * Description: Hardware initialize
 *
 *************************************************************************/
int SysInit(void)
{
  // Initialize the system
#ifdef FLASH
  if (SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, 0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0))
    return 1;
#else
  if (SYS_Init(FOSC, FCCLK, VPBDIV1, USER_RAM,   0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0))
    return 1;
#endif

  // Initialize Timer 0
  PM_OpenPeripheral(PC_TIMER0);
  if (TIMER_Init(TIMER0, TIMER_PRECISION))
    return 1;
  // Initialize Timer 1
  PM_OpenPeripheral(PC_TIMER1);
  if (TIMER_Init(TIMER1, TIMER_PRECISION))
    return 1;

  // initialize VIC
  VIC_Init();
  VIC_SetProtectionMode(UserandPrivilegedMode);

  // Timer0 interrupt
  VIC_SetVectoredIRQ(TIMER0_ISR,VIC_Slot2,VIC_TIMER0);
  VIC_EnableInt(1<<VIC_TIMER0);

  // Timer1 interrupt
  VIC_SetVectoredIRQ(TIMER1_ISR,VIC_Slot3,VIC_TIMER1);
  VIC_EnableInt(1<<VIC_TIMER1);

  // Initialize USB
  // HID USB
  HidInit();

  // Enable interrupts non vectored interrupts
  VIC_DisableNonVectoredIRQ();

  return 0;
}

/*************************************************************************
 * Function Name: UserStart
 * Parameters: none
 * Return: none
 *
 * Description: User demo initialize subroutine
 *
 *************************************************************************/
void UserStart(void)
{
  /* Buttons Init */
  ButtonsInit();

  /* ADC Init */
  ADC_Init();
  /* get current voltage on CH3 */
  AdcHold = ADC_Measure();

  /* System time init */
  TIMER_SetMatchAction(TIMER0, CH0, TimerAction_Interrupt | TimerAction_ResetTimer,
  1sec_T0/TICK_PER_SECOND, SetSysTickFlag, (void *)&TickSysFlag, DONOTHING);
  TIMER_Start(TIMER0);

  // Init LCD Backlight
  LCD_BACKLIGHT_INIT();

  /* HD44780 LCD driver init */
  HD44780_PowerUpInit();

  // LCD Backlight On
  LCD_BACKLIGHT_ON();
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 * Return: int
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main (void)
{
LPC_INT8U X_Temp;
LPC_INT16U AdcCurr;
LPC_INT32S Y_Temp = 0;
signed char X = 0, Y = 0;

  if(SysInit() == 0)
  {
    // Start user program
    __enable_interrupt();
    UserStart();
  }

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Show message on LCD
  HD44780_StrShow(1, 1,  "IAR Systems ARM ");
  HD44780_StrShow(1, 2,  "HID Class Mouse ");

  // Main loop
  while(1)
  {

    if(TickSysFlag)
    {
      TickSysFlag = 0;

      /* Measure Current voltage level */
      AdcCurr = ADC_Measure();
      if (abs((LPC_INT16S)AdcCurr - (LPC_INT16S)AdcHold) > MIN_CHANGE || Y_Temp)
      {
        /* Calculate difference between preview and current value */
        Y_Temp += AdcHold - AdcCurr;
        AdcHold = AdcCurr;
        /* Avoid overflow */
        if(Y_Temp > 0)
        {
          if(Y_Temp > SCHAR_MAX)
          {
            Y = SCHAR_MAX;
            Y_Temp -= SCHAR_MAX;
          }
          else
          {
            Y = Y_Temp;
            Y_Temp = 0;
          }
        }
        else if (Y_Temp < 0)
        {
          if(Y_Temp < -127)
          {
            Y = -127;
            Y_Temp -= -127;
          }
          else
          {
            Y = Y_Temp;
            Y_Temp = 0;
          }
        }
      }

      /* Buttons event */
      X_Temp = GetButtonsEvent();
      if (X_Temp)
      {
        /* Button1 event "Left" */
        if (X_Temp&BUTT_EVENT_1)
        {
          X = -10;
        }
        /* Button2 event "Right" */
        if (X_Temp&BUTT_EVENT_2)
        {
          X = 10;
        }
      }
    }

    /* Send new offsets */
    if(Y || X)
    {
      UsbWakeUp();
      HidMouseSendReport(X,Y,0);
      Y = X = 0;
    }
  }
}
