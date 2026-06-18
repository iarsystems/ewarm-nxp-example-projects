#include "lpc11Uxx_adc.h"
#include "glcd_api.h"
#include "stdio.h"


void ADCExp()
{
  uint32_t adc_value;
  char buf[6];
  /* Configuration for ADC :
   *  Frequency at 1Mhz
   *  ADC channel 2, no Interrupt
   */
  ADC_Init(1000000);
  //ADC_IntConfig(ADC_ADINTEN5,ENABLE);
  ADC_PinsInit(ADC_CHANNEL_0);
  ADC_ChannelCmd(ADC_CHANNEL_0,ENABLE);
  while(1)
  {
    /* Start conversion */
    ADC_StartCmd(ADC_START_NOW);
    /* Wait conversion complete */
    while (!(ADC_ChannelGetStatus(ADC_CHANNEL_0,ADC_DATA_DONE)));
    adc_value = ADC_ChannelGetData(ADC_CHANNEL_0);
    sprintf((char *)buf,"ADC Ch-0:%.4d",(int)adc_value);
    init_xy_axis(PAGE0 + 6,COL0);
    lcd_display_string((unsigned char *)(char *)buf);
  }
}
