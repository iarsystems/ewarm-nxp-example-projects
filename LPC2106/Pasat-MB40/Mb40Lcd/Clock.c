// -------------------------------------------------------------------
// -----------------------LPC2106 RTC  routines-----------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board by PASAT
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */

//#include "ma_uart0.h"    /* UART0 Module driver header file */
//#include "ma_uart0.h"    /* UART1 Module driver header file */
#include "ma_gpio.h"     /* GPIO  Module driver header file */
#include "ma_spi.h"      /* SPI   Module driver header file */
#include "ma_i2c.h"      /* I2C   Module driver header file */
#include "ma_pcb.h"     /* PCB Module driver header file */

#include "string.h"
#include "stdio.h"
#include "mainmb40.h"
#include "ma_rtc.h"
#include "Clock.h"
#include "my_files.h"

void Init_My_Clock(void)
{
	MA_RtcTime MyInfo;

	MA_Init_RTC();
	//MA_ResetCTC_RTC(); // OK ????????????????

	MyInfo.MA_Sec=0;
	MyInfo.MA_Min=0;
	MyInfo.MA_Hour=0;
	MyInfo.MA_Mday=25;
	MyInfo.MA_Mon=6;
	MyInfo.MA_Wday=4;
	MyInfo.MA_Yday=1;
	MyInfo.MA_Year=2004;
	MA_SetTime_RTC(MyInfo);

	MA_Start_RTC();
}


//      Time structure with current time counter values:
//      struct {
//        int MA_Sec;     Second value - [0,59] 
//        int MA_Min;     Minute value - [0,59]
//        int MA_Hour;    Hour value - [0,23]
//        int MA_Mday;    Day of the month value - [1,31]
//        int MA_Mon;     Month value - [1,12]
//        int MA_Year;    Year value - [0,4095]
//        int MA_Wday;    Day of week value - [0,6]
//        int MA_Yday;    Day of year value - [1,365]
//      } MA_RtcTime;                

void Exec_ShowTime(void)
{
	char TextStr[10];
	char *TimePtr;
	U8	pChar;
	S8 Result8;
	int SecondVal;
	MA_RtcTime MyInfo;

	Jump_Date_Here:

	MyInfo = MA_GetTime_RTC();

	TimePtr = &TextStr[0];
	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Year:  ");
	sprintf(TimePtr,"%4d",MyInfo.MA_Year);
	My_PutString(TimePtr);

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Month: ");
	//sprintf(TimePtr,"%2d",MyInfo.MA_Mon);
	//My_PutString(TimePtr);
	switch (MyInfo.MA_Mon)
	{
		case  1: TimePtr="January";break;
		case  2: TimePtr="February";break;
		case  3: TimePtr="March";break;
		case  4: TimePtr="April";break;
		case  5: TimePtr="May";break;
		case  6: TimePtr="June";break;
		case  7: TimePtr="July";break;
		case  8: TimePtr="August";break;
		case  9: TimePtr="September";break;
		case 10: TimePtr="October";break;
		case 11: TimePtr="November";break;
		case 12: TimePtr="December";break;
	}
	//My_PutString("  ");
	My_PutString(TimePtr);
	TimePtr = &TextStr[0];

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Day:   ");
	//sprintf(TimePtr,"%2d",MyInfo.MA_Wday);
	//My_PutString(TimePtr);
	switch (MyInfo.MA_Wday)
	{
		case 0: TimePtr="Monday";break;
		case 1: TimePtr="Tuesday";break;
		case 2: TimePtr="Wednesday";break;
		case 3: TimePtr="Thursday";break;
		case 4: TimePtr="Friday";break;
		case 5: TimePtr="Saturday";break;
		case 6: TimePtr="Sunday";break;
	}
	//My_PutString("  ");
	My_PutString(TimePtr);
	TimePtr = &TextStr[0];

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Time:  ");

		MyInfo = MA_GetTime_RTC();
		sprintf(TimePtr,"%.2d",MyInfo.MA_Hour);
		My_PutString(TimePtr);
		My_PutChar('-');
		sprintf(TimePtr,"%.2d",MyInfo.MA_Min);
		My_PutString(TimePtr);
		My_PutChar('-');
		sprintf(TimePtr,"%.2d",MyInfo.MA_Sec);
		My_PutString(TimePtr);
		SecondVal = MyInfo.MA_Sec;

	do
	{
		MyInfo = MA_GetTime_RTC();
		if (SecondVal != MyInfo.MA_Sec)
		{
	    	My_Put_Some(KEY_LEFT,8);
			sprintf(TimePtr,"%.2d",MyInfo.MA_Hour);
			My_PutString(TimePtr);
			My_PutChar('-');
			sprintf(TimePtr,"%.2d",MyInfo.MA_Min);
			My_PutString(TimePtr);
			My_PutChar('-');
			sprintf(TimePtr,"%.2d",MyInfo.MA_Sec);
			My_PutString(TimePtr);
			SecondVal = MyInfo.MA_Sec;
		}
	
	    Result8=My_WaitForChar(&pChar);
		if (Result8) pChar=0;

		if (pChar== KEY_ENTER)
		{
			My_PutChar(0xD);
			My_PutChar(0xA);
			Exec_SetTime();
			My_PutChar(0xD);
			My_PutChar(0xA);
			pChar=0;
			goto Jump_Date_Here;
		}	
	}
	while (pChar != KEY_ESC);

	My_PutChar(0xD);
	My_PutChar(0xA);

}


void Exec_SetTime(void)
{
	char TextStr[10];
	char *TimePtr;
	U8 MyKey,MyHours,MyMinutes;
	MA_RtcTime MyInfo;

	TimePtr = &TextStr[0];

	My_PutChar(0xD);
	My_PutChar(0xA);
	My_PutString(" Set Hours:   ");
	MyInfo = MA_GetTime_RTC();
	sprintf(TimePtr,"%.2d",MyInfo.MA_Hour);
	My_PutString(TimePtr);
	My_Put_Some(KEY_LEFT,2);
	MyHours=Arrow_Up_Down(MyInfo.MA_Hour,0,23,&MyKey);
	My_PutChar(0xD);
	My_PutChar(0xA);
	if (MyKey==KEY_ESC) return;

	My_PutString(" Set Minutes: ");
	MyInfo = MA_GetTime_RTC();
	sprintf(TimePtr,"%.2d",MyInfo.MA_Min);
	My_PutString(TimePtr);
	My_Put_Some(KEY_LEFT,2);
	MyMinutes=Arrow_Up_Down(MyInfo.MA_Min,0,59,&MyKey);
	My_PutChar(0xD);
	My_PutChar(0xA);
	if (MyKey==KEY_ESC) return;

	MyInfo.MA_Sec=0;
	MyInfo.MA_Hour=MyHours;
	MyInfo.MA_Min=MyMinutes;
	MA_Stop_RTC();
	MA_SetTime_RTC(MyInfo);
	MA_Start_RTC();
	return;
}


U8 Arrow_Up_Down(U8 Value,U8 ValueMin,U8 ValueMax,U8 *MyKey)
//U8 Arrow_Up_Down(U8 Value,ValueMin,ValueMax,*MyKey)
{
	char TextStr[10];
	char *TextPtr;
	U8 Data8,Result;

	TextPtr = &TextStr[0];
 	do
 	{	//entering data byte - 2 hex digits,Space,Backspace
 		Result=My_WaitForChar(&Data8);
 		if (Result) 
 		{
 			Data8=0;
			continue;
		}
 
 		if (Data8==KEY_LEFT)
 		{
 			if (Value<ValueMin+1)
 				Value=ValueMax;
 			else
 				Value--;
 		}

 		if (Data8==0x20)
 		{
 			if (Value>ValueMax-1)
 				Value=ValueMin;
 			else
 				Value++;
 		}

 		sprintf(TextPtr,"%.2d",Value);
 		My_PutString(TextPtr);
 		My_Put_Some(KEY_LEFT,2);
 	}
 	while ((Data8 != KEY_ENTER) & (Data8 != KEY_ESC));

	*MyKey=Data8;
	return Value;
}

