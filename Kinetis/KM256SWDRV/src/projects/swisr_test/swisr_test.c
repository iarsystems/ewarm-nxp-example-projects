/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * swisr_test.c
 ******************************************************************************/
#include "drivers.h"

static void sw0_callback  (void);
static void sw1_callback  (void);

/******************************************************************************/
void main (void)
{
  /* install software interrupt handler(s)                                    */
  SWISR_HandlerInit (0, PRI_LVL2, sw0_callback);
  SWISR_HandlerInit (1, PRI_LVL2, sw1_callback);

  /* enable interrupts on global level                                        */
  EnableInterrupts();

  /* call first handler                                                       */
  SWISR_HandlerCountCall (0,2);   /* skip first call                          */
  SWISR_HandlerCountCall (0,2);   /* skip second call                         */
  SWISR_HandlerCountCall (0,2);   /* only this call will trigger sw0_callback */

  while(1);
}

/******************************************************************************
 * Software ISR callback                                                      *
 ******************************************************************************/
static void sw0_callback  (void)
{
  SWISR_HandlerCall (1);          /* trigger now sw1_callback                 */
}

static void sw1_callback  (void)
{
  SWISR_HandlerCall (0);          /* trigger now sw0_callback                 */
}
