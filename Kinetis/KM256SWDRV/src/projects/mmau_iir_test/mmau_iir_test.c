/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * mmau_iir_test.c
 ******************************************************************************/
#if defined(__CROSSWORKS_ARM)
  #include <cross_studio_io.h>
#else
  #include <stdio.h>
#endif
#include "drivers.h"

/* mmau callback prototype (optional)                                         */
static void mmau_callback (MMAU_CALLBACK_TYPE type);

/* Filter coefficients computed by Matlab script: [b,a]=butter(4,5/64)        */
static const frac32 a[] =
{
  FRAC32(3.3594/8.0),FRAC32(-4.2755/8.0),FRAC32(2.4390/8.0),FRAC32(-0.5256/8.0)
};

static const frac32 b[] =
{
  FRAC32(1.6777E-4/8.0),FRAC32(6.7107E-4/8.0),FRAC32(0.0010/8.0),
  FRAC32(6.7107E-4/8.0),FRAC32(1.6777E-4/8.0)
};

/* Filter previous input values                                               */
static frac32 x[] ={0l, 0l, 0l, 0l};

/* Filter previous output values                                              */
static frac32 y[] ={0ll, 0ll, 0ll, 0ll};

/****************************************************************************//*!
 * @brief  Compute 4th order infinite impulse response filter (IIR) iteration:
 *         y(n) = b(0)*x(n)+ b(1)*x(n-1)+ b(2)*x(n-2)+ b(3)*x(n-3)+b(4)*x(n-4)
 *                +a(1)*y(n-1)+a(2)*y(n-2)+a(3)*y(n-3)+a(4)*y(n-4)
 *         Internal accumulations don't saturate. The IIR filter output is within
 *         32-bit fractional range from 0x80000000 to 0x7fffffff.
 * @param  x  - Input fractional value represented in 32-bit fractional format
 *              "x(n)".
 * @param *pb - Pointer to filter constants "b" represented in 32-bit fractional
 *              format "b(0) ... b(4)".
 * @param *pa - Pointer to filter constants "a" represented in 32-bit fractional
 *              format "-a(1) ... -a(4)".
 * @param sc  - Filter constants scaling.
 * @param *px - Pointer to previous input values represented in 32-bit fractional
 *              format "x(n-1) ... x(n-4)".
 * @param *py - Pointer to previous output values represented in 32-bit fractional
 *              format "y(n-1) ... y(n-4)".
 * @return  Function returns 32-bit value in fractional format.
 *******************************************************************************/
static frac32 l_iir_4ord (frac32 x, const frac32 *pb, const frac32 *pa, int16 sc,
                          frac32 *px, frac32 *py)
{
  register frac32 y;

  /* actual filter output value calculation with using MMAU instructions      */
  mul_ll(*(pb  ),      x);                      /* acc=    b[0]*x[0]          */
  mac_ll(*(pb+1),*(px  ));                      /* acc=acc+b[1]*x[1]          */
  mac_ll(*(pb+2),*(px+1));                      /* acc=acc+b[2]*x[2]          */
  mac_ll(*(pb+3),*(px+2));                      /* acc=acc+b[3]*x[3]          */
  mac_ll(*(pb+4),*(px+3));                      /* acc=acc+b[4]*x[4]          */
  mac_ll(*(py  ),*(pa  ));                      /* acc=acc+a[1]*y[1]          */
  mac_ll(*(py+1),*(pa+1));                      /* acc=acc+a[2]*y[2]          */
  mac_ll(*(py+2),*(pa+2));                      /* acc=acc+a[3]*y[3]          */
  y = l_mac_ll(*(py+3),*(pa+3))<<sc;            /* y  =(acc+a[4]*y[4])<<sc    */

  /* shifting previous input values                                           */
  *(px+3)=*(px+2); *(px+2)=*(px+1); *(px+1)=*(px); *(px)= x;

  /* shifting previous output values                                          */
  *(py+3)=*(py+2); *(py+2)=*(py+1); *(py+1)=*(py); *(py)= y;

  return y;
}

/* Input array - sine() signal cycle sampled in 128 points                    */
static const frac32 input[] =
{
         0,  2166732,  2753973,  1604297,    123267,    12239,   1734704,  4097830,
   5326223,  4660528,  3040692,  2258324,   3341806,  5629024,   7363103,  7274451,
   5723831,  4376659,  4701958,  6648280,   8689462,  9199008,   7914891,  6175140,
   5711947,  7090722,  9200346, 10253866,   9398102,  7482051,   6291825,  6941379,
   8868840, 10342644, 10021506,  8162831,   6390892,  6233630,   7746287,  9461846,
   9712938,  8134287,  5992130,  5043834,   5955090,  7700647,   8488467,  7374730,
   5114698,  3482901,  3675087,  5231628,   6452309,  5928792,   3814157,  1685767,
   1125013,  2293441,  3788057,  3906151,   2180158,  -200227,  -1458963,  -832815,
    741949,  1473959,   331515, -2027032,  -3845491, -3851097,  -2400263, -1156602,
  -1591289, -3656810, -5828926, -6479050,  -5340656, -3748612,  -3435831, -4971342,
  -7246945, -8474926, -7797983, -6060970,  -5049184, -5879437,  -7993407, -9660172,
  -9535189, -7871223, -6290283, -6322041,  -8025478, -9935534, -10382821, -8997469,
  -7042261, -6274946, -7364674, -9289208, -10255966, -9317117,  -7223680, -5749155,
  -6092036, -7796394, -9162986, -8780521,  -6797545, -4789092,  -4338241, -5610457,
  -7205110, -7417939, -5777092, -3468902,  -2269827, -2946762,  -4566901, -5338900,
  -4227529, -1887148,   -73038,   -60914,  -1498499, -2723782,  -2263234,  -160231
};
static frac32 output[sizeof(input)/sizeof(input[0])];
static volatile uint32 ticks;

#pragma optimize=no_inline
void main (void)
{
  register int i;

  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  SYST_Init (SYST_MODULE_FREERUN_TMR_DIV1_EN_CONFIG,PRI_LVL0,NULL);

  /* initialize mmau module (optional)                                        */
  MMAU_InstallCallback (MMAU_DZIE_EN_VIE_EN_QIE_EN_CONFIG, PRI_LVL0, mmau_callback);

  /* enable interrupts                                                        */
  EnableInterrupts();

  for(i=0; i < (sizeof(input)/sizeof(input[0])); i++)
  {
    SYST_ClrCntrVal  ();

    output[i] = l_iir_4ord (input[i], b, a, 3, (frac32*)x, (frac32*)y);

    ticks = SYST_GetCntrVal ();

    #if defined(__CROSSWORKS_ARM)
      debug_printf("%ld %ld\n",input[i],output[i]);
    #else
      printf("%ld %ld\n",input[i],output[i]);
    #endif
  }

  while(1);
}

/* mmau callback definition                                                   */
static void mmau_callback (MMAU_CALLBACK_TYPE type)
{
  if (type == DZIF_CALLBACK) { __asm volatile ("nop"); }
  if (type ==  VIF_CALLBACK) { __asm volatile ("nop"); }
  if (type ==  QIF_CALLBACK) { __asm volatile ("nop"); }
}
