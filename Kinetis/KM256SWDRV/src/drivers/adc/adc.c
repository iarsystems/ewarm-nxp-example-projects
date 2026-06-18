/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      adc.c
 * @version   1.0.4.1
 * @date      Nov-23-2015
 * @brief     Analog-to-Digital Converter (ADC) driver source code.
 ******************************************************************************/
#include "common.h"
#include "adc.h"

#if !defined(MCU_MKM34Z7)
  #error "The adc.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static ADC_CALLBACK pCallbackADC;

/******************************************************************************
 * public function definitions                                                *
 ******************************************************************************/
void ADC_Init  (tADC adc, uint8 avg, tADC_CH CHA, tADC_CH CHB, tADC_CH CHC,
                tADC_CH CHD, uint8 ip, ADC_CALLBACK pCallback)
{
  if (pCallback) { pCallbackADC = pCallback; NVIC_SetIsr(INT_ADC0,ip); }
  ADC0_CFG2 = adc.CFG2;
  ADC0_CFG1 = adc.CFG1;
  ADC0_CV1  = adc.CV1;
  ADC0_CV2  = adc.CV2;
  ADC0_SC2  = adc.SC2;
  if (avg != HWAVG_OFF) { adc.SC3 |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(avg)); }
  ADC0_SC3  = adc.SC3;
  ADC0_SC1B = CHB.SC1;
  ADC0_SC1C = CHC.SC1;
  ADC0_SC1D = CHD.SC1;
  ADC0_SC1A = CHA.SC1;
}

#define TWO_POWER_N_SAMPLES  6  /* select carefully - COP Timer not updated   */
int ADC_ExecCalib (tADC adc, tADC_CALIB *pCALIB)
{
  long tmp=0;
  unsigned long tmp32[7]={0,0,0,0,0,0,0};
  register uint16 i=0, numLoops = ((uint16)1<<TWO_POWER_N_SAMPLES);

  ADC0_CFG1 = adc.CFG1;
  ADC0_CFG2 = adc.CFG2;
  ADC0_SC2  = (adc.SC2 & (~ADC_SC2_ADTRG_MASK));

  /* calibration loop                                                         */
  while ((i++) < numLoops)
  {
    /* The input channel, conversion mode continuous function, compare        */
    /* function, resolution mode, and differential/single-ended mode are all  */
    /* ignored during the calibration function.                               */
    ADC0_SC1A = 0x1f;
    ADC0_SC3  = (adc.SC3 & (~ADC_SC3_CAL_MASK) & (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3)));
    ADC0_SC3 |= ADC_SC3_CAL_MASK;
    while (!ADC_Ready (CHA));
    if (ADC0_SC3 & ADC_SC3_CALF_MASK) { ADC0_SC3 |= ADC_SC3_CALF_MASK; return 1; }

    tmp+= (short int)ADC0_OFS;

    tmp32[0]+=(unsigned long)ADC0_CLP0;
    tmp32[1]+=(unsigned long)ADC0_CLP1;
    tmp32[2]+=(unsigned long)ADC0_CLP2;
    tmp32[3]+=(unsigned long)ADC0_CLP3;
    tmp32[4]+=(unsigned long)ADC0_CLP4;
    tmp32[5]+=(unsigned long)ADC0_CLPS;
    tmp32[6]+=(unsigned long)ADC0_CLPD;
  }
  pCALIB->OFS  = tmp >> TWO_POWER_N_SAMPLES;

  pCALIB->CLP[0] = tmp32[0] >> TWO_POWER_N_SAMPLES;
  pCALIB->CLP[1] = tmp32[1] >> TWO_POWER_N_SAMPLES;
  pCALIB->CLP[2] = tmp32[2] >> TWO_POWER_N_SAMPLES;
  pCALIB->CLP[3] = tmp32[3] >> TWO_POWER_N_SAMPLES;
  pCALIB->CLP[4] = tmp32[4] >> TWO_POWER_N_SAMPLES;
  pCALIB->CLPS   = tmp32[5] >> TWO_POWER_N_SAMPLES;
  pCALIB->CLPD   = tmp32[6] >> TWO_POWER_N_SAMPLES;

  tmp = tmp32[0]+tmp32[1]+tmp32[2]+tmp32[3]+tmp32[4]+tmp32[5];
  pCALIB->PG  = ((tmp>>(1+TWO_POWER_N_SAMPLES))|0x8000);

  ADC_SaveCalib (pCALIB);         /* update calibration registers             */

  return 0;
}

void ADC_SaveCalib (tADC_CALIB *pCALIB)
{
  ADC0_CLP0 = pCALIB->CLP[0];
  ADC0_CLP1 = pCALIB->CLP[1];
  ADC0_CLP2 = pCALIB->CLP[2];
  ADC0_CLP3 = pCALIB->CLP[3];
  ADC0_CLP4 = pCALIB->CLP[4];
  ADC0_CLPS = pCALIB->CLPS;
  ADC0_CLPD = pCALIB->CLPD;
  ADC0_PG   = pCALIB->PG;
  ADC0_OFS  = pCALIB->OFS;
}

/******************************************************************************
 * interrupt function definitions                                             *
 ******************************************************************************/
weak void adc_isr (void)
{
  if (ADC0_SC1A & ADC_SC1_COCO_MASK) { pCallbackADC (CHA_CALLBACK, ADC0_RA); }
  if (ADC0_SC1B & ADC_SC1_COCO_MASK) { pCallbackADC (CHB_CALLBACK, ADC0_RB); }
  if (ADC0_SC1C & ADC_SC1_COCO_MASK) { pCallbackADC (CHC_CALLBACK, ADC0_RC); }
  if (ADC0_SC1D & ADC_SC1_COCO_MASK) { pCallbackADC (CHD_CALLBACK, ADC0_RD); }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
