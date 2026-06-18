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
 * Jumpers settings
 * On Main board
 * J13 - 2-3
 * J14 - 2-3
 * J48 - filled
 * J49 - 3-4
 * J50 - 5-6
 * J58 - 1-2
 * J59 - 1-2
 *
 * On CPU board
 * J3  - 1-2
 * J4  - 1-2
 * J24 - filled
 *
 *    $Revision: 28 $
 *
**************************************************************************/
#include "includes.h"

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
  if (TIMER_Init(TIMER0, TIMER_PRECISION))
    return 1;
  // Initialize Timer 1
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
  /* System time init */
  TIMER_SetMatchAction(TIMER0, CH0, TimerAction_Interrupt | TimerAction_ResetTimer,
  1sec_T0/TICK_PER_SECOND, SetSysTickFlag, (void *)&TickSysFlag, DONOTHING);
  TIMER_Start(TIMER0);
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
Int32U State, Count;

  if(SysInit() == 0)
  {
    // Start user program
    __enable_interrupt();
    UserStart();
  }

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Main loop
  while(1)
  {

    if(TickSysFlag)
    {
      TickSysFlag = 0;
      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured &&
         !UsbCoreReq(UsbCoreReqDevSusState))
      {
        // Mouse pointer
        switch(State)
        {
        case 0:
          HidMouseSendReport(-2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 1:
          HidMouseSendReport(0,2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 2:
          HidMouseSendReport(2,0,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        case 3:
          HidMouseSendReport(0,-2,0);
          if(Count-- == 0)
          {
            State++;
            Count = 100;
          }
          break;
        default:
          State = 0;
          Count = 100;
        }
      }
      else
      {
        // Reset state
        State = 0;
        Count = 100;
      }
    }
  }
}
