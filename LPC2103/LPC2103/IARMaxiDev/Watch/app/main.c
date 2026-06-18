/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        :
 *       Author      : Stanimir Bonev
 *       Description : 3, January 2006
 *
 * Jumpers:
 *  EXT/JLINK - the correct jumper position depends on the power source used
 *  DBG       - install the jumper
 *  INT0      - install the jumper
 *  INT1      - install the jumper
 *  INT2      - install the jumper
 * Buttons:
 *  INT0      - Decrement/Backlight On
 *  INT1      - Correct time/Backlight On
 *  INT2      - Increment/Backlight On
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

#define DLY_100US       100
#define STARTUP_DLY     2
#define LIGHT_DLY       8
#define CORRECT_TO_DLY  5

#define KEY_DOWN        16
#define KEY_UP          15
#define KEY_SET         14

#define CURSOR_X_HOUR   6
#define CURSOR_Y_HOUR   1
#define CURSOR_X_MINUTE 9
#define CURSOR_Y_MINUTE 1
#define CURSOR_X_YEAR   8
#define CURSOR_Y_YEAR   2
#define CURSOR_X_MONTH  12
#define CURSOR_Y_MONTH  2
#define CURSOR_X_DAY    15
#define CURSOR_Y_DAY    2

typedef enum
{
  CLOCK_IDLE = 0, SET_HOUR, SET_MINUTE, SET_YEAR, SET_MONTH, SET_DAY,
} ClockState_t;

volatile Boolean KeyUpPress, KeyDownPress, KeySetPress;

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
 * Function Name: KeyUpIntHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Key UP interrupt handler
 *
 *************************************************************************/
void KeyUpIntHandler (void)
{
  EXTINT = 4;
  KeyUpPress = TRUE;
  VICVectAddr = 0;
}

/*************************************************************************
 * Function Name: KeySetIntHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Key SET interrupt handler
 *
 *************************************************************************/
void KeySetIntHandler (void)
{
  EXTINT = 2;
  KeySetPress = TRUE;
  VICVectAddr = 0;
}

/*************************************************************************
 * Function Name: KeyDownIntHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Key DOWN interrupt handler
 *
 *************************************************************************/
void KeyDownIntHandler (void)
{
  EXTINT = 1;
  KeyDownPress = TRUE;
  VICVectAddr = 0;
}

/*************************************************************************
 * Function Name: KeysInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Keys
 *
 *************************************************************************/
void KeysInit (void)
{
  // Init variables
  KeyUpPress = KeyDownPress = KeySetPress = FALSE;
  // Set to EXT Interrupt
  PINSEL0_bit.P0_14 = 1;
  PINSEL0_bit.P0_15 = 1;
  PINSEL1_bit.P0_16 = 1;
  // Set ext. interrupt edge to sensitive mode
  EXTMODE = 0x7;
  // Falling Edge
  EXTPOLAR = 0;
  // Set interrupts
  // Assign to IRQ
  VICIntSelect_bit.EINT0 = 0;
  VICIntSelect_bit.EINT1 = 0;
  VICIntSelect_bit.EINT2 = 0;
  // Set slots
  VICVectAddr0 = (Int32U) KeyDownIntHandler;
  VICVectCntl0_bit.NUMBER = VIC_EINT0;
  VICVectCntl0_bit.ENABLED = TRUE;
  VICVectAddr1 = (Int32U) KeySetIntHandler;
  VICVectCntl1_bit.NUMBER = VIC_EINT1;
  VICVectCntl1_bit.ENABLED = TRUE;
  VICVectAddr2 = (Int32U) KeyUpIntHandler;
  VICVectCntl2_bit.NUMBER = VIC_EINT2;
  VICVectCntl2_bit.ENABLED = TRUE;
  // Clear pending interrupts
  EXTINT = 0x7;
  // Enable interrupts
  VICIntEnable_bit.EINT0 = 1;
  VICIntEnable_bit.EINT1 = 1;
  VICIntEnable_bit.EINT2 = 1;
}

/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Dly
 *
 * Return: none
 *
 * Description: Delay ~ (arg * 100us)
 *
 *************************************************************************/
void DelayResolution100us(Int32U Dly)
{
  for(; Dly; Dly--)
  {
    for(volatile Int32U j = DLY_100US; j; j--)
    {
    }
  }
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
int main(void)
{
Rtc_DateTime_t Rtc_DateTime;      // Hold Time and Date
ClockState_t ClockState;          // Watch state
Boolean LcdUpdate;                // LCD text update flag
Boolean ClockUpdate;              // Clock and Data change flag
Boolean CursorOn;                 // Cursor state flag
HD44780_XY_DEF CursorX, CursorY;  // Hold cursor position
Int8S Line1[17], Line2[17];       // Line 1,2
Int32U Dly;
Int32U LightOnDly;                // Backlight time out
Int32U CorrectToDly;              // Correct mode time out
Int8U DayOfMonthMax;              // Hold max day of month
Int32U CheckDly;

  // Init VIC
  __disable_interrupt();
  // Disable PLL
  PLLCON = 0;
  // Write Feed
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  // Set periphery divider /4
  APBDIV_bit.APBDIV  = 0;
  // Memory map init flash memory is mapped on 0 address
  MEMMAP_bit.MAP = 1;
  // Assign all interrupt channels to IRQ
  VICIntSelect  =  0;
  // Disable all interrupts
  VICIntEnClear = 0xFFFFFFFF;
  // Clear all software interrupts
  VICSoftIntClear = 0xFFFFFFFF;
  // VIC registers can be accessed in User or privileged mode
  VICProtection = 0;
  // Clear interrupt
  VICVectAddr = 0;
  // Clear address of the Interrupt Service routine (ISR) for non-vectored IRQs.
  VICDefVectAddr = 0;
  // Clear address of the Interrupt Service routine (ISR) for vectored IRQs.
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
  // Init GPIO
  KeysInit();
  // Init RTI
  RtcInit();
  // Init LCD
  __enable_interrupt();
  HD44780_PowerUpInit();
  // Init variables
  HD44780SetBackLight(TRUE);
  ClockUpdate = LcdUpdate = CursorOn = FALSE;
  ClockState  = CLOCK_IDLE;
  // Show text on LCD
  HD44780_StrShow(1,1," IAR Systems AB ");
  HD44780_StrShow(1,2,"LPC2103 KS Card ");
  for(Dly = STARTUP_DLY; Dly; Dly--)
  {
    CheckDly = 0x100000;
    while(!RtcSecIntFlag())
    {
      if(--CheckDly == 0)
      {
        RtcReset();
        break;
      }
    }
  }
  LightOnDly = LIGHT_DLY;
  // main loop
  while(1)
  {
    // Check for time change
    if(RtcSecIntFlag())
    {
      ClockUpdate = TRUE;
      // Back light control
      if(LightOnDly)
      {
        if(--LightOnDly == 0)
        {
          HD44780SetBackLight(FALSE);
        }
        else
        {
          HD44780SetBackLight(TRUE);
        }
      }
      // Setting Time out
      if(CorrectToDly)
      {
        if(--CorrectToDly == 0)
        {
          ClockState = CLOCK_IDLE;
        }
      }
    }
    switch (ClockState)
    {
    case CLOCK_IDLE:  // Idle state
      if(ClockUpdate)
      {
        ClockUpdate = FALSE;
        CursorOn  = FALSE;
        // Get current time and date
        RtcGetTimeDate(&Rtc_DateTime);
        // Time and Data update
        LcdUpdate = TRUE;
      }
      if(KeySetPress)
      {
        // Enter in watch set mode
        KeySetPress = FALSE;
        // Set state (HOUR setting)
        ClockState  = SET_HOUR;
        // Update LDC text
        LcdUpdate   = TRUE;
        // Cursor on
        CursorOn    = TRUE;
        // Position of cursor
        CursorX     = CURSOR_X_HOUR;
        CursorY     = CURSOR_Y_HOUR;
        // Back light on
        LightOnDly  = LIGHT_DLY;
        // Set time out of setting mode
        CorrectToDly= CORRECT_TO_DLY;
      }
      else if(KeyUpPress || KeyDownPress)
      {
        // Clear key flags
        KeyUpPress = KeyDownPress = FALSE;
        // Backlight on
        LightOnDly = LIGHT_DLY;
      }
      break;
    case SET_HOUR:  // Set Hour
      if(KeySetPress)
      {
        KeyUpPress = KeyDownPress = KeySetPress = FALSE;
        ++ClockState;
        CursorX     = CURSOR_X_MINUTE;
        CursorY     = CURSOR_Y_MINUTE;
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if(KeyUpPress)
      {
        KeyUpPress = KeyDownPress = FALSE;
        if(++Rtc_DateTime.hour > RTC_HOURMAX)
        {
          Rtc_DateTime.hour = RTC_HOURMIN;
        }
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      else if (KeyDownPress)
      {
        if(--Rtc_DateTime.hour > RTC_HOURMAX)
        {
          Rtc_DateTime.hour = RTC_HOURMAX;
        }
        KeyDownPress = FALSE;
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      break;
    case SET_MINUTE:  // Set Minute
      if(KeySetPress)
      {
        KeyUpPress = KeyDownPress = KeySetPress = FALSE;
        ++ClockState;
        CursorX     = CURSOR_X_YEAR;
        CursorY     = CURSOR_Y_YEAR;
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if(KeyUpPress)
      {
        KeyUpPress = KeyDownPress = FALSE;
        if(++Rtc_DateTime.minute > RTC_MINUTEMAX)
        {
          Rtc_DateTime.minute = RTC_MINUTEMIN;
        }
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      else if (KeyDownPress)
      {
        if(--Rtc_DateTime.minute > RTC_MINUTEMAX)
        {
          Rtc_DateTime.minute = RTC_MINUTEMAX;
        }
        KeyDownPress = FALSE;
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      break;
    case SET_YEAR:  // Set Year
      if(KeySetPress)
      {
        KeyUpPress = KeyDownPress = KeySetPress = FALSE;
        ++ClockState;
        CursorX     = CURSOR_X_MONTH;
        CursorY     = CURSOR_Y_MONTH;
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if(KeyUpPress)
      {
        KeyUpPress = KeyDownPress = FALSE;
        if(++Rtc_DateTime.year > RTC_YEARMAX)
        {
          Rtc_DateTime.year = RTC_YEARMIN;
        }
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if (KeyDownPress)
      {
        KeyDownPress = FALSE;
        if(--Rtc_DateTime.year > RTC_YEARMAX)
        {
          Rtc_DateTime.year = RTC_YEARMAX;
        }
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      break;
    case SET_MONTH: // Set Month
      if(KeySetPress)
      {
        KeyUpPress = KeyDownPress = KeySetPress = FALSE;
        ++ClockState;
        CursorX     = CURSOR_X_DAY;
        CursorY     = CURSOR_Y_DAY;
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if(KeyUpPress)
      {
        KeyUpPress = KeyDownPress = FALSE;
        if(++Rtc_DateTime.month > RTC_MONTHMAX)
        {
          Rtc_DateTime.month = RTC_MONTHMIN;
        }
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if (KeyDownPress)
      {
        KeyDownPress = FALSE;
        if(--Rtc_DateTime.month < RTC_MONTHMIN)
        {
          Rtc_DateTime.month = RTC_MONTHMIN;
        }
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      break;
    case SET_DAY: // Set Day of month
      if((Rtc_DateTime.month == 2) && IsLeapYear(Rtc_DateTime.year))
      {
        DayOfMonthMax = 29;
      }
      else
      {
        DayOfMonthMax = RTC_MonthVal[Rtc_DateTime.month];
      }
      if(KeySetPress)
      {
        KeyUpPress  = KeyDownPress = KeySetPress = FALSE;
        RtcSetTimeDate(&Rtc_DateTime);
        ClockState  = CLOCK_IDLE;
        CursorOn    = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if(KeyUpPress)
      {
        KeyUpPress = KeyDownPress = FALSE;
        if(++Rtc_DateTime.day > DayOfMonthMax)
        {
          Rtc_DateTime.day = 1;
        }
        Rtc_DateTime.DOW = GetDOW(Rtc_DateTime.year,Rtc_DateTime.month ,Rtc_DateTime.day);
        LcdUpdate   = TRUE;
        CorrectToDly= CORRECT_TO_DLY;
        LightOnDly  = LIGHT_DLY;
      }
      else if (KeyDownPress)
      {
        KeyDownPress = FALSE;
        if(--Rtc_DateTime.day < 1)
        {
          Rtc_DateTime.day = DayOfMonthMax;
        }
        Rtc_DateTime.DOW = GetDOW(Rtc_DateTime.year,Rtc_DateTime.month ,Rtc_DateTime.day);
        LcdUpdate    = TRUE;
        CorrectToDly = CORRECT_TO_DLY;
        LightOnDly   = LIGHT_DLY;
      }
      break;
    default:
      ClockState = CLOCK_IDLE;
    } // switch (ClockState)
    // Show Time and date
    if(LcdUpdate)
    {
      LcdUpdate = FALSE;
      // Cursor off
      HD44780_CursorPosSet(FALSE,FALSE,1,1);
      // Prepare fist line of text
      sprintf((char*)Line1,"    %2d:%.2d:%.2d    ",
              Rtc_DateTime.hour,Rtc_DateTime.minute,Rtc_DateTime.second);
      // Prepare second line of text
      sprintf((char*)Line2,"%s %4d %s %2d ",
              RTC_DOWTbl[Rtc_DateTime.DOW],Rtc_DateTime.year,
              RTC_MonthTbl[Rtc_DateTime.month],Rtc_DateTime.day);
      // Show text
      HD44780_StrShow(1,1,Line1);
      HD44780_StrShow(1,2,Line2);
      if(CursorOn)
      {
        // Display cursor
        HD44780_CursorPosSet(TRUE,TRUE,CursorX,CursorY);
      } // if(CursorOn)
    } // if(LcdUpdate)
  } // while(1)
} // main(void)


/* These handlers trap instruction fiq, prefetch abort, undefined instruction abort
   and data fetch abort exceptions
*/
__fiq __arm void fiq_handler(void)
{
  while(1){}
}

__arm void undef_handler(void)
{
  while(1);
}

__arm void prefetch_handler(void)
{
  while(1);
}

__arm void data_handler(void)
{
  while(1);
}
