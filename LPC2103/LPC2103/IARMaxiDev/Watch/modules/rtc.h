/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : rtc.h
 *    Description : Definition of RTC
 *
 *    History :
 *    1. Date        : 4, January 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4271 $
**************************************************************************/
#include "includes.h"

#ifndef __RTC_H
#define __RTC_H

#ifdef RTC_GOBALS
#define RTC_EXTERN
#else
#define RTC_EXTERN extern
#endif

#define RTC_YEARMIN         1901
#define RTC_YEARMAX         2099

#define RTC_MONTHMIN        1
#define RTC_MONTHMAX        12

#define RTC_DOWMIN          0
#define RTC_DOWMAX          6

#define RTC_HOURMIN         0
#define RTC_HOURMAX         23

#define RTC_MINUTEMIN       0
#define RTC_MINUTEMAX       59

#define RTC_SECONDMIN       0
#define RTC_SECONDMAX       59

// 1901.1.1 DOW = Tuesday
#define RTC_BASEYEAR        1901
#define RTC_BASEMONTH       1
#define RTC_BASEDAY         1
#define RTC_BASEDOW         2

typedef enum
{
	January = 1,February,March,April,May,June,July,
	August,September,October,November,December,
} RTC_Month_t;

static char * const RTC_DOWTbl[] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

static char * const RTC_MonthTbl[] = {
	"",
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

static const int RTC_MonthVal[]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef struct {
	unsigned short year;  // year value
	unsigned char month;  // month value
	unsigned char day;    // day value
	unsigned char hour;   // hour value
	unsigned char minute; // minute value
	unsigned char second; // second value
	/* the below member is used in get-operation */
	unsigned char DOW;    // Day of week
	unsigned char DOY;    // Day of year	
} Rtc_DateTime_t, *pRtc_DateTime_t;

/*************************************************************************
 * Function Name: IsLeapYear
 * Parameters: Int16U
 *
 * Return: Boolean
 *
 * Description: Judge whether the specifying year is leap year.
 *
 *************************************************************************/
Boolean IsLeapYear (Int16U year);

/*************************************************************************
 * Function Name: GetDOY
 * Parameters: Int16U year, Int8U month, Int8U day
 *
 * Return: Int32U
 *
 * Description: Get the day of year according to the date
 *
 *************************************************************************/
Int32U GetDOY (Int16U year, Int8U month, Int8U day);

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
Int32U GetDOW (Int16U year, Int8U month, Int8U day);

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
Boolean IsValidDay (Int16U year, Int8U month, Int8U day);

/*************************************************************************
 * Function Name: RtcSetTimeDate
 * Parameters: none
 *
 * Return: none
 *
 * Description: Disable RTC
 *
 *************************************************************************/
inline void RtcClockDisable (void);

/*************************************************************************
 * Function Name: RtcClockEnable
 * Parameters: none
 *
 * Return: none
 *
 * Description: Enable RTC
 *
 *************************************************************************/
inline void RtcClockEnable (void);

/*************************************************************************
 * Function Name: RtcReset
 * Parameters: none
 *
 * Return: none
 *
 * Description: Reset RTC module (set default mode, time and date)
 *
 *************************************************************************/
void RtcReset (void);

/*************************************************************************
 * Function Name: RtcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init RTC module
 *
 *************************************************************************/
void RtcInit (void);

/*************************************************************************
 * Function Name: RtcSetTimeDate
 * Parameters: pRtc_DateTime_t pRtc_DateTime
 *
 * Return: Boolean
 *		      FALSE -- not valid date
 *		      TRUE -- valid date
 *
 * Description: Set Time and Date
 *
 *************************************************************************/
Boolean RtcSetTimeDate (pRtc_DateTime_t pRtc_DateTime);

/*************************************************************************
 * Function Name: RtcGetTimeDate
 * Parameters: pRtc_DateTime_t pRtc_DateTime
 *
 * Return: none
 *
 * Description: Get time and date
 *
 *************************************************************************/
void RtcGetTimeDate (pRtc_DateTime_t pRtc_DateTime);

/*************************************************************************
 * Function Name: RtcSecIntFlag
 * Parameters: none
 *
 * Return: Boolean
 *
 * Description: Return seconds interrupt flag
 *
 *************************************************************************/
Boolean RtcSecIntFlag (void);

#endif // __RTC_H
