/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * tmr_test.c
 ******************************************************************************/
#include "drivers.h"

void GPTimerEventHander(TMR_CH_CALLBACK_TYPE type, int32 result);
void GeneralPurposeTimerInit(void);

uint8 OneSecTimer=0;

/*
 Quad Timer 1 Interrupt Frequency(ms)
*/
#define TIMER_MOD                 25

void main (void)
{
  /* initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);
  /* clock mode 2:1:1, 24MHz                                                  */
  FLL_Init  (FLL_MODULE_FEE_24MHZ_CONFIG);

  /*Initializing the GPIO in Output mode to check the Timer functionality     */
  PORT_Init(PORTD,PORT_MODULE_LED_MODE,PIN1);
  GPIO_Init(GPIOD,GPIO_OUT_LOGIC0_MODE,PIN1);

  GeneralPurposeTimerInit();

  OneSecTimer=40; /* This will be one second Timer. =40*25=1000ms             */

  EnableInterrupts();

  while(1);
}

/*
  Quad Timer will be used as a General Purpose Timer.
  Presently it will be configured for 25 ms interrupt frequency.
  Using this Timer various Timer counter can be derived.
 */

void GeneralPurposeTimerInit()
{
  uint16 ModValue = 0;
  uint32 BusClk=12000000;
  /*
  Initializing the Configurable Timer
  It will use Quad Timer 1
  Enable the Quad Timer Clock
  Interrupt Frequency Desired= 25ms
  Interrupt Frequency in terms of Bus Clock= (Interrupt Time*Bus Freq)
  Configuring the Timer in Free Running Mode
  */
  ModValue = BusClk/1000 ;
  ModValue = ModValue / 128;
  ModValue=ModValue* TIMER_MOD;
  TMR_CH1_Init(TMR_CH_CNTR_EN_CONFIG4(BUS_CLK_DIV128,SEC_CNTR0_INP),
      ModValue, 0x0000, 0x0000, ModValue, 0x0000, 1,
      &GPTimerEventHander);

}

void GPTimerEventHander(TMR_CH_CALLBACK_TYPE type, int32 result)
{
  if(type==COMPARE_CALLBACK)
  {
    /*Toggling GPIO at a frequency of 1sec    */
    if(OneSecTimer)
    {
      OneSecTimer--;
      if(OneSecTimer==0)
      {
        GPIO_Tgl(GPIOD,PIN1);
        OneSecTimer=40;
      }
    }
  }
}
