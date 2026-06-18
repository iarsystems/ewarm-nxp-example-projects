#ifndef _RTC_API_H_
#define _RTC_API_H_
#include "lpc_types.h"

#define RTC_NO_ERROR      0

/*  ERROR CODES   */
#define RTC_ERR_NO_RESPONSE   1
#define RTC_ERR_WRONG_PARAM   2

typedef struct
{
  uint8_t year;    //-- from 00
  uint8_t month;   //-- 1..12
  uint8_t day;     //-- 1..31
  uint8_t hour;    //-- 0..23
  uint8_t minute;  //-- 0..59
  uint8_t second;  //-- 0..59
}DATETIMEINFO;

//int init_rtc(char * dti);
int pcf8523t_set(uint8_t * dti);
int pcf8523t_get(uint8_t * dti);
#endif
