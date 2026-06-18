/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmau_test.c
 ******************************************************************************/
#include "drivers.h"

/* mmau callback prototype (optional)                                         */
static void mmau_callback (MMAU_CALLBACK_TYPE type);

static frac32 volatile result;

static const frac64 sin_coef[] =
{
  FRAC64( 0.50000000000000), FRAC64(-0.82246703342411), FRAC64( 0.40587121264168),
  FRAC64(-0.09537591206104), FRAC64( 0.01307392390883), FRAC64(-0.00117304051773),
  FRAC64( 0.00006930000000) /* 0.00007421439652 */
};

/****************************************************************************//*!
 * @brief  Compute sine of x.
 * @param  x  - Input arguments x = 0x80000000 to 0x7fffffff, corresponds to the
 *              angle -pi to pi.
 * @return  Function returns sine of input angle in range -1 to 1.
 *******************************************************************************/
static frac32 sin (frac32 x)
{
  if      (x > FRAC32( 0.5)) { x = FRAC32( 1.0)-x; }
  else if (x < FRAC32(-0.5)) { x = FRAC32(-1.0)-x; }
  mul_dl (sin_coef[6],x);                         /* acc=    x*sin_coef[6]    */
  maca_dl(sin_coef[5],x);                         /* acc=acc*x+sin_coef[5]    */
  mula_l (            x);                         /* acc=acc*x                */
  maca_dl(sin_coef[4],x);                         /* acc=acc*x+sin_coef[4]    */
  mula_l (            x);                         /* acc=acc*x                */
  maca_dl(sin_coef[3],x);                         /* acc=acc*x+sin_coef[3]    */
  mula_l (            x);                         /* acc=acc*x                */
  maca_dl(sin_coef[2],x);                         /* acc=acc*x+sin_coef[2]    */
  mula_l (            x);                         /* acc=acc*x                */
  maca_dl(sin_coef[1],x);                         /* acc=acc*x+sin_coef[1]    */
  mula_l (            x);                         /* acc=acc*x                */
  maca_dl(sin_coef[0],x);                         /* acc=acc*x+sin_coef[0]    */
  mula_l (            x);                         /* acc=acc*x                */
  return l_mula_l (FRAC32(0.78539816339745))<<3;  /* acc=acc*2*pi             */
}

static volatile uint32 ticks;

void main (void)
{
  /* initialize mmau module (optional)                                        */
  MMAU_InstallCallback (MMAU_DZIE_EN_VIE_EN_QIE_EN_CONFIG, PRI_LVL0, mmau_callback);

  /* enable interrupts                                                        */
  EnableInterrupts();

  SYST_Init (SYST_MODULE_FREERUN_TMR_DIV1_EN_CONFIG,PRI_LVL0,NULL);

  SYST_ClrCntrVal  ();

  result = sin (FRAC32( 1.0));
  result = sin (FRAC32( 0.8));
  result = sin (FRAC32( 0.6));
  result = sin (FRAC32( 0.4));
  result = sin (FRAC32( 0.2));
  result = sin (FRAC32( 0.0));
  result = sin (FRAC32(-0.2));
  result = sin (FRAC32(-0.4));
  result = sin (FRAC32(-0.6));
  result = sin (FRAC32(-0.8));
  result = sin (FRAC32(-1.0));

  ticks = SYST_GetCntrVal ();

  while(1);
}

/* mmau callback definition                                                   */
static void mmau_callback (MMAU_CALLBACK_TYPE type)
{
  if (type == DZIF_CALLBACK) { __asm volatile ("nop"); }
  if (type ==  VIF_CALLBACK) { __asm volatile ("nop"); }
  if (type ==  QIF_CALLBACK) { __asm volatile ("nop"); }
}
