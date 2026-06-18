/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * irtc_test.c
 ******************************************************************************/
#include <string.h>
#include "drivers.h"

/* Initialize time variable for 20.10.2012 Saturday 23:59:58                  */
static tTIME  wtime = {58,59,23,20,SATURDAY,OCTOBER,2012}, rtime;
static tALARM atime = {35, 0, 0,21,OCTOBER,2012};
static tTAMPER_LOG log;
static void irtc_callback (IRTC_CALLBACK_TYPE type, void *data);

void main (void)
{
  IRTC_Init (IRTC_MODULE_COMP_OFF_CONFIG(LFSR_CLK_64HZ,10),
             IRTC_TAMPER_PIN_DI_CONFIG,
             IRTC_TAMPER_PIN_PASSIVE_POL_HIGH_CONFIG(LFSR_CLK_64HZ,10),
             IRTC_TAMPER_PIN_DI_CONFIG,
             IRTC_TAMPER1_MASK|IRTC_ALM_MASK|IRTC_1HZ_MASK);
  IRTC_InstallCallback (PRI_LVL3,irtc_callback);
  IRTC_SetClock (&wtime);
  IRTC_SetDST   (2,25,MARCH,3,28,OCTOBER);
  IRTC_SetAlarm (ALM_MATCH_SMH,&atime);
  EnableInterrupts();

  while(1);
}

static void irtc_callback (IRTC_CALLBACK_TYPE type, void *data)
{
  if(type == IRTC_1HZ_CALLBACK    ) { memcpy (&rtime,data,sizeof(tTIME));     }
  if(type == IRTC_ALM_CALLBACK    ) { IRTC_SetClock(&wtime);                  }
  if(type == IRTC_TAMPER1_CALLBACK) { memcpy (&log,data,sizeof(tTAMPER_LOG)); }
}
