/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : rtc.c
 *    Description : RTC module
 *
 *    History :
 *    1. Date        : 4, January 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 28 $
**************************************************************************/
#include "rtc.h"

/*************************************************************************
 * Function Name: IsLeapYear
 * Parameters: Int16U
 *
 * Return: Boolean
 *
 * Description: Judge whether the specifying year is leap year.
 *
 *************************************************************************/
Boolean IsLeapYear (Int16U year)
{
  if (!(year%4) && (year%100) || !(year%400))
  {
    return (TRUE);
  }
  return (FALSE);
}

/*************************************************************************
 * Function Name: GetDOY
 * Parameters: Int16U year, Int8U month, Int8U day
 *
 * Return: Int32U
 *
 * Description: Get the day of year according to the date
 *
 *************************************************************************/
Int32U GetDOY (Int16U year, Int8U month, Int8U day)
{
int DOY=0, i;

  for(i=1; i<month; i++)
  {
    DOY+=RTC_MonthVal[i];
  }
  if (month > 2)
  {
    if (IsLeapYear(year))
    {
      DOY++;
    }
  }
  return (DOY+day);	
}

/*************************************************************************
 * Function Name: GetDOW
 * Parameters: Int16U year, Int8U month, Int8U day
 *
 * Return: Int32U -- (0~6)
 *
 * Description: Get the day of week according to the date.
 *
 * NOTE: Year is not smaller than RTC_YEARMIN (1901).
 *
 *************************************************************************/
Int32U GetDOW (Int16U year, Int8U month, Int8U day)
{
Int32U i, DOW;

  for (i = RTC_BASEYEAR, DOW = 0; i < year; i++)
  {
    DOW +=365;
    if  (IsLeapYear(i))
    {
      DOW++;
    }
  }
  DOW += GetDOY (year, month, day) - 1;
  DOW  =(DOW + RTC_BASEDOW) % 7;
  return (DOW);	
}

/*************************************************************************
 * Function Name: IsValidDay
 * Parameters: Int16U year, Int8U month, Int8U day
 *
 * Return: Boolean
 *		      FALSE -- not valid day
 *		      TRUE -- valid day
 *
 * Description: Check if the specify day is valid
 *
 *************************************************************************/
Boolean IsValidDay (Int16U year, Int8U month, Int8U day)
{
  /* Valid Judge */
  if (year < RTC_YEARMIN || year > RTC_YEARMAX ||day <1)
  {
    return(FALSE);
  }

  switch(month)
  {
  case January:
  case March:
  case May:
  case July:
  case August:
  case October:
  case December:
    if (day > 31)
    {
      return (FALSE);
    }
    break;
  case April:
  case June:
  case September:
  case November:
    if (day > 30)
    {
      return (FALSE);
    }
    break;
  case February:
    if (IsLeapYear(year))
    {
      if (day > 29)
      {
        return(FALSE);
      }
    }
    else
    {
      if (day>28)
      {
        return (FALSE);
      }
    }
    break;
  default:
    return (FALSE);
  }
  return (TRUE);
}

/*************************************************************************
 * Function Name: RtcReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset RTC module (set default mode, time and date)
 *
 *************************************************************************/
void RtcReset (void)
{
  // Stop counters clock and clear
  CCR   = 0x12;
  // Init default time and date
  // 12:00:00 1,January 2006, Sunday
  DOM   = 1;
  MONTH = 1;
  YEAR  = 2006;
  DOW   = 0;
  DOY   = 1;
  HOUR  = 12;
  MIN   = 0;
  SEC   = 0;
  // Disable alarms interrupts
  AMR   = 0;
  // Disable Counter Increment interrupts
  CIIR  = 1;
  // Enable clock
  CCR   = 0x11;
}

/*************************************************************************
 * Function Name: RtcSetTimeDate
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable RTC
 *
 *************************************************************************/
inline void RtcClockDisable (void)
{
  CCR_bit.CLKEN = FALSE;
}

/*************************************************************************
 * Function Name: RtcClockEnable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable RTC
 *
 *************************************************************************/
inline void RtcClockEnable (void)
{
  CCR_bit.CLKEN = TRUE;
}

/*************************************************************************
 * Function Name: RtcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init RTC module
 *
 *************************************************************************/
void RtcInit (void)
{
Rtc_DateTime_t Rtc_DateTimeCheck;
  // Clocking RTC
  PCONP_bit.PCRTC = 1;
  // Clear all pending interrupts
  ILR = 0x3;
  // Get current state of RTC Data and time registers
  RtcGetTimeDate(&Rtc_DateTimeCheck);
  // Check RTC init state
  if(!IsValidDay(Rtc_DateTimeCheck.year,
                 Rtc_DateTimeCheck.month,
                 Rtc_DateTimeCheck.day) ||
      (CCR & 0x1F) != 0x11)
  {
    // Reset RTC (set default mode, time and date)
    RtcReset();
  }
}

/*************************************************************************
 * Function Name: RtcSetTimeDate
 * Parameters: pRtc_DateTime_t pRtc_DateTime
 *
 * Return: Boolean
 *		      FALSE -- not valid date
 *		      TRUE  -- valid date
 *
 * Description: Set Time and Date
 *
 *************************************************************************/
Boolean RtcSetTimeDate (pRtc_DateTime_t pRtc_DateTime)
{
  // Valid Judge
  if (!IsValidDay(pRtc_DateTime->year, pRtc_DateTime->month, pRtc_DateTime->day))
  {
    return (FALSE);
  }
  // Calculate DOW, DOY
  pRtc_DateTime->DOY = GetDOY(pRtc_DateTime->year, pRtc_DateTime->month, pRtc_DateTime->day);
  pRtc_DateTime->DOW = GetDOW(pRtc_DateTime->year, pRtc_DateTime->month, pRtc_DateTime->day);
  // Stop clock
  RtcClockDisable();
  // Set date and time
  DOM   = pRtc_DateTime->day;
  MONTH = pRtc_DateTime->month;
  YEAR  = pRtc_DateTime->year;
  DOW   = pRtc_DateTime->DOW;
  DOY   = pRtc_DateTime->DOY;
  HOUR  = pRtc_DateTime->hour;
  MIN   = pRtc_DateTime->minute;
  SEC   = pRtc_DateTime->second;
  // Enable clock
  RtcClockEnable();
  return (TRUE);
}

/*************************************************************************
 * Function Name: RtcGetTimeDate
 * Parameters: pRtc_DateTime_t pRtc_DateTime
 *
 * Return: none
 *
 * Description: Get time and date
 *
 *************************************************************************/
void RtcGetTimeDate (pRtc_DateTime_t pRtc_DateTime)
{
  while(1)
  {
    pRtc_DateTime->second = SEC;
    pRtc_DateTime->minute = MIN;
    pRtc_DateTime->hour   = HOUR;
    pRtc_DateTime->day    = DOM;
    pRtc_DateTime->month  = MONTH;
    pRtc_DateTime->year   = YEAR;
    pRtc_DateTime->DOW    = DOW;
    pRtc_DateTime->DOY    = DOY;
    // Check for time and data change
    if(SEC == pRtc_DateTime->second)
    {
      break;
    }
  }
}

/*************************************************************************
 * Function Name: RtcSecIntFlag
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Return seconds interrupt flag
 *
 *************************************************************************/
Boolean RtcSecIntFlag (void)
{
  if(ILR_bit.RTCCIF)
  {
    ILR_bit.RTCCIF = 1;
    return (TRUE);
  }
  return (FALSE);
}
