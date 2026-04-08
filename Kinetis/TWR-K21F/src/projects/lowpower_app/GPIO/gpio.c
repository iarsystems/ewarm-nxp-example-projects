/*
 * gpio.c
 *
 *  Created on: Jul 30, 2012
 *      Author: B23243
 */
#include "gpio.h"


void vfnLEDsOutPutConfig(void)
{
#ifdef LED1
	  LED1_PCR = PCR_OUTPUT_CONFIG;
	  LED1_OFF;
	  LED1_OUTPUT_EN;
#endif
	  
	    
#ifdef LED2
	  LED2_PCR = PCR_OUTPUT_CONFIG;
	  LED2_OFF;
	  LED2_OUTPUT_EN;
#endif	  
}

void vfnLEDsInPutConfig(void)
{	
#ifdef LED1
	  LED1_ON;
	  LED1_PCR = PCR_INPUT_CONFIG;
	  LED1_INPUT_EN;
#endif
	  
	    
#ifdef LED2
	  LED2_ON;
	  LED2_PCR = PCR_INPUT_CONFIG;
	  LED2_INPUT_EN;
#endif	  
}
