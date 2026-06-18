/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : ntc.h
 *    Description : NTC module include file
 *
 *    History :
 *    1. Date        : July 28, 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef  __NTC_H
#define  __NTC_H

#include <math.h>
#include "arm_comm.h"

Flo32 NtcRatioToTemperature (Flo32 Ratio);
Flo32 NtcTemperatureToRatio (Flo32 Ratio);

#endif  /* __NTC_H */
