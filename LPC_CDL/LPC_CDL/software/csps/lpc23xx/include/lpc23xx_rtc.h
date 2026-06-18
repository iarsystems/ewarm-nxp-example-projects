/***********************************************************************
* $Id: lpc23xx_rtc.h 28 2014-01-27 14:45:13Z danielru $
*
* Project: LPC23xx Real time clock definitions
*
* Description:
*     This file contains the structure definitions and manifest
*     constants for the LPC23xx chip family component: Real time clock
*
***********************************************************************
* Software that is described herein is for illustrative purposes only  
* which provides customers with programming information regarding the  
* products. This software is supplied "AS IS" without any warranties.  
* NXP Semiconductors assumes no responsibility or liability for the 
* use of the software, conveys no license or title under any patent, 
* copyright, or mask work right to the product. NXP Semiconductors 
* reserves the right to make changes in the software without 
* notification. NXP Semiconductors also make no representation or 
* warranty that such application will be suitable for the specified 
* use without further testing or modification. 
**********************************************************************/
#ifndef LPC23XX_RTC_H_
#define LPC23XX_RTC_H_

#include "arm7tdmis_types.h"
#include "lpc23xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
* RTC register structures
**********************************************************************/
typedef struct 
{
	__IO U32 ILR;				/* Interrupt Location Register */
	__IO U32 CTC;				/* Clock Tick Counter */
	__IO U32 CCR;				/* Clock Control Register */
	__IO U32 CIIR;				/* Counter Increment Interrupt Register */
	__IO U32 AMR;				/* Alarm Mask Register */
	__IO U32 CTIME0;			/* Consolidated Time Register 0 */
	__IO U32 CTIME1;			/* Consolidated Time Register 1 */
	__IO U32 CTIME2;			/* Consolidated Time Register 2 */
	__IO U32 SEC;				/* Seconds Register */
	__IO U32 MIN;				/* Minutes Register */
	__IO U32 HOUR;				/* Hours Register */
	__IO U32 DOM;				/* Day of Month Register */
	__IO U32 DOW;				/* Day of Week Register */
	__IO U32 DOY;				/* Day of Year Register */
	__IO U32 MONTH;				/* Months Register */
	__IO U32 YEAR;				/* Years Register */
	__IO U32 CISS;				/* Counter Increment select mask for 
								Sub-Second interrupt */
	__IO U32 RESERVED[7];		/* Reserved */
	__IO U32 ALSEC;				/* Alarm value for Seconds */
	__IO U32 ALMIN;				/* Alarm value for Minutes */
	__IO U32 ALHOUR;			/* Alarm value for Hours */
	__IO U32 ALDOM;				/* Alarm value for Day of Month */
	__IO U32 ALDOW;				/* Alarm value for Day of Week */
	__IO U32 ALDOY;				/* Alarm value for Day of Year */
	__IO U32 ALMON;				/* Alarm value for Months */
	__IO U32 ALYEAR;			/* Alarm value for Year */
	__IO U32 PREINT;			/* Prescaler value, integer portion */
	__IO U32 PREFRAC;			/* Prescaler value, integer portion */
} RTC_Type;

/**********************************************************************
* Time struct definitions for easy manipulate the data
**********************************************************************/
typedef struct 
{
	__IO U32 SEC;				/* Seconds Register */
	__IO U32 MIN;				/* Minutes Register */
	__IO U32 HOUR;				/* Hours Register */
	__IO U32 DOM;				/* Day of Month Register */
	__IO U32 DOW;				/* Day of Week Register */
	__IO U32 DOY;				/* Day of Year Register */
	__IO U32 MONTH;				/* Months Register */
	__IO U32 YEAR;				/* Years Register */
} RTC_TIME_Type;


/**********************************************************************
* ILR register definitions
**********************************************************************/
/* Bit inform the source interrupt is counter increment*/
#define RTC_IRL_CIF			_BIT(0)
/* Bit inform the source interrupt is alarm match*/
#define RTC_IRL_ALF			_BIT(1)
/* Bit inform the Counter Increment Sub-Seconds match */
#define RTC_IRL_RTSSF		_BIT(2)

/**********************************************************************
* CISS register definition
**********************************************************************/
/* An interrupt is generated on every 16 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_16		_SBF(2,0x00)
/* An interrupt is generated on every 32 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_32		_SBF(2,0x01)
/* An interrupt is generated on every 64 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_64		_SBF(2,0x02)
/* An interrupt is generated on every 128 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_128		_SBF(2,0x03)
/* An interrupt is generated on every 256 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_256		_SBF(2,0x04)
/* An interrupt is generated on every 512 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_512		_SBF(2,0x05)
/* An interrupt is generated on every 1024 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_1024		_SBF(2,0x06)
/* An interrupt is generated on every 2048 counts of the Clock Tick Counter */
#define RTC_CISS_SUBSECSEL_2048		_SBF(2,0x07)

/**********************************************************************
* CTCR register value definitions
**********************************************************************/
/* Clock Tick Counter value mask */
#define	RTC_CTCR_VALUE(REG)		((REG&0xFFFE)>>1)

/**********************************************************************
* CCR register definitions
**********************************************************************/
/* Clock enable */
#define RTC_CCR_CLKEN			_BIT(0)
/* Clock reset */
#define RTC_CCR_CTCRST			_BIT(1)
/* Clock test 0 */
#define RTC_CCR_CTTEST_0		_BIT(2)
/* Clock test 1 */
#define RTC_CCR_CTTEST_1		_BIT(3)
/* Clock source selection bit */
#define RTC_CCR_CLKSRC			_BIT(4)


/**********************************************************************
* CIIR register definitions
**********************************************************************/
/* Counter Increment Interrupt bit for second */
#define RTC_CIIR_SECEN			_BIT(0)
/* Counter Increment Interrupt bit for minute */
#define RTC_CIIR_MINEN			_BIT(1)
/* Counter Increment Interrupt bit for hour */
#define RTC_CIIR_HOUREN			_BIT(2)
/* Counter Increment Interrupt bit for day of month */
#define RTC_CIIR_DOMEN			_BIT(3)
/* Counter Increment Interrupt bit for day of week */
#define RTC_CIIR_DOWEN			_BIT(4)
/* Counter Increment Interrupt bit for day of year */
#define RTC_CIIR_DOYEN			_BIT(5)
/* Counter Increment Interrupt bit for month */
#define RTC_CIIR_MONEN			_BIT(6)
/* Counter Increment Interrupt bit for year */
#define RTC_CIIR_YEAREN			_BIT(7)

/**********************************************************************
* AMR register definitions
**********************************************************************/
/* Counter Increment Select Mask bit for second */
#define RTC_AMR_SECEN		_BIT(0)
/* Counter Increment Select Mask bit for minute */
#define RTC_AMR_MINEN		_BIT(1)
/* Counter Increment Select Mask bit for hour */
#define RTC_AMR_HOUREN		_BIT(2)
/* Counter Increment Select Mask bit for day of month */
#define RTC_AMR_DOMEN		_BIT(3)
/* Counter Increment Select Mask bit for day of week */
#define RTC_AMR_DOWEN		_BIT(4)
/* Counter Increment Select Mask bit for day of year */
#define RTC_AMR_DOYEN		_BIT(5)
/* Counter Increment Select Mask bit for month */
#define RTC_AMR_MONEN		_BIT(6)
/* Counter Increment Select Mask bit for year */
#define RTC_AMR_YEAREN		_BIT(7)

/**********************************************************************
*  Prescale register definitions
**********************************************************************/
/* Prescale Value, integer portion mask */
#define RTC_PREINT					(0x1FFF)
/* Prescale Value, integer portion mask value */
#define RTC_PREINT_VAL(REG)			(REG & (0x1FFF))
/* Prescale Value, fractional portion mask */
#define RTC_PREFRAC					(0x3FFF)
/* Prescale Value, fractional portion mask value */
#define RTC_PREFRAC_VAL(REG)		(REG & (0x3FFF))

/* Macro pointing to Real time clock registers */
#define RTC ((RTC_Type *)(RTC_BASE))

#ifdef __cplusplus
}
#endif

#endif /*LPC23XX_RTC_H_*/
