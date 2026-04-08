/******************************************************************************
*    LAB3 - Analog-To-Digital Converter (ADC) Module                          *
*                                                                             *
*       This lab demonstrates how to use the ADC module to read the analog    *
*    value of the on-board light sensor. The application reads the            *
*    light sensor,adc channel 3, value and sets it to the LEDs in its binary  *
*    representation.                                                          *
*   Add or remove light to the sensor to see the LEDs change the read value.  *
******************************************************************************/



/**********************************************************************************************
* External objects
**********************************************************************************************/
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"
#include "GPIO.h"
#include "ADC.h"
/**********************************************************************************************
* Global variables
**********************************************************************************************/


/**********************************************************************************************
* Constants and macros
**********************************************************************************************/
#define LED0_TOGGLE		OUTPUT_TOGGLE(PTC,PTC0)
#define LED1_TOGGLE		OUTPUT_TOGGLE(PTC,PTC1)
#define LED2_TOGGLE		OUTPUT_TOGGLE(PTC,PTC2)
#define LED3_TOGGLE		OUTPUT_TOGGLE(PTC,PTC3)

#define LED0_OFF		OUTPUT_CLEAR(PTC,PTC0);
#define LED1_OFF		OUTPUT_CLEAR(PTC,PTC1);
#define LED2_OFF		OUTPUT_CLEAR(PTC,PTC2);
#define LED3_OFF		OUTPUT_CLEAR(PTC,PTC3);

#define LED0_ON			OUTPUT_SET(PTC,PTC0);
#define LED1_ON			OUTPUT_SET(PTC,PTC1);
#define LED2_ON			OUTPUT_SET(PTC,PTC2);
#define LED3_ON			OUTPUT_SET(PTC,PTC3);

#define RED_ON                  OUTPUT_CLEAR(PTH,PTH0);
#define RED_OFF                 OUTPUT_SET(PTH,PTH0);
#define GREEN_ON                OUTPUT_CLEAR(PTH,PTH1);
#define GREEN_OFF               OUTPUT_SET(PTH,PTH1);
#define BLUE_ON                 OUTPUT_CLEAR(PTE,PTE7);
#define BLUE_OFF                OUTPUT_SET(PTE,PTE7);


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/
void Enable_Interrupt(UINT8 vector_number);

/**********************************************************************************************
* Local variables
**********************************************************************************************/


/**********************************************************************************************
* Local functions
**********************************************************************************************/
extern void KBI_Init(void);
extern void KBI_SetCallback(pt2FuncU8);
void KBI_Interrupt(UINT8 ch);

/**********************************************************************************************
* Global functions
**********************************************************************************************/
/***********************************************************************************************
*
* @brief    GPIO_Init - Initialize the LEDs as outputs
* @param    none
* @return   none
*
************************************************************************************************/
void GPIO_Init()
{
	CONFIG_PIN_AS_GPIO(PTE,PTE7,OUTPUT); /* Configure blue LED (PTE7) as an output */
	CONFIG_PIN_AS_GPIO(PTH,PTH0,OUTPUT); /* Configure red LED (PTH0) as an output */
	CONFIG_PIN_AS_GPIO(PTH,PTH1,OUTPUT); /* Configure green LED (PTH1) as an output */

	CONFIG_PIN_AS_GPIO(PTE,PTE4,INPUT); /* Configure SW1 (PTE4) as an input */
	CONFIG_PIN_AS_GPIO(PTE,PTE5,INPUT); /* Configure SW2 (PTE5) as an input */
	ENABLE_INPUT(PTE,PTE4);		    /* Enable input SW1*/
	ENABLE_INPUT(PTE,PTE5);	           /*  Enable input SW2*/

	LED0_OFF;							/* Turn off LED0 */
	LED1_OFF;							/* Turn off LED1 */
	LED2_OFF;							/* Turn off LED2 */
	LED3_OFF;							/* Turn off LED3 */
}


/***********************************************************************************************
*
* @brief    main() - Program entry function
* @param    none
* @return   none
*
************************************************************************************************/
void main(void)
{
	UINT32 counter = 0;
	UINT8 adc_value = 0;
        
        Enable_Interrupt(25);           //Enable the Port E interrupt (User Buttons 1 and 2 are PortE.4 and PortE.5, respectively)

	Clk_Init();			    /* Configure clocks to run at 20 Mhz */
	GPIO_Init();			/* Configure button pins as inputs and LED pins as outputs */
	ADC_Init(10,EIGTH_BIT);	        /* Configure ADC channel 10 in 8-bit resolution mode */
	KBI_Init();						/* Initialize KBI module */
	KBI_SetCallback(KBI_Interrupt);	/* Set the KBI callback function to be called on each button press */
	Enable_Interrupt(INT_KBI1); 	/* Enable KBI1 Interrupts */

	for(;;) {
	   	counter++;

	    adc_value=ADC_Read(10); /* Read ADC value from channel 10 */

	    /* Display ADC value in LED*/
            if(adc_value <= 41)
            {//If the potentiometer is in the first sixth of its range, display red
              RED_ON;
              GREEN_OFF;
              BLUE_OFF;
            }
            else if(adc_value <= 82)
            {//If the potentiometer is in the second sixth of its range, display orange
              RED_ON;
              GREEN_ON;
              BLUE_OFF;
            }
            else if(adc_value <= 123)
            {//If the potentiometer is in the third sixth of its range, display yellow
              RED_OFF;
              GREEN_ON;
              BLUE_ON;
            }
            else if(adc_value <= 164)
            {//If the potentiometer is in the fourth sixth of its range, display green
              RED_OFF;
              GREEN_ON;
              BLUE_OFF;
            }
            else if(adc_value <= 205)
            {//If the potentiometer is in the fifth sixth of its range, display blue
              RED_OFF;
              GREEN_OFF;
              BLUE_ON;
            }
            else
            {//Else the potentiometer is in the last sixth of its range, display violet
              RED_ON;
              GREEN_OFF;
              BLUE_ON;
            }
	}
}

void delay()
{
  volatile int loopCounter = 0xFFFF;
  
  while(loopCounter--)
  {
  }
}
/***********************************************************************************************
*
* @brief    KBI_Interrupt(UINT8 ch). Callback function called by the KBI module on each interrupt
* @param    KBI channel
* @return   none
*
************************************************************************************************/
void KBI_Interrupt(UINT8 ch)
{
  int cycleColors = 0;
  
		switch(ch)
		{
		case 0:
                        cycleColors = 1;
		break;
		case 1:
			RED_ON;
                        GREEN_ON;
                        BLUE_ON;
                        cycleColors =  0;
		break;
		case 2:
			RED_OFF;
                        GREEN_OFF;
                        BLUE_OFF;
                        cycleColors =  0;
		break;
		}
                
                while(!READ_INPUT(PTE,PTE4) || !READ_INPUT(PTE,PTE5))
                {//Stay here until the user lets off the button
                  if(cycleColors)
                  {
			RED_ON;                 //Display red color
                        GREEN_OFF;
                        BLUE_OFF;
                        delay();
                        
			RED_ON;                 //Display orange color
                        GREEN_ON;
                        BLUE_OFF;
                        delay();
                        
			RED_OFF;                 //Display yellow color
                        GREEN_ON;
                        BLUE_ON;
                        delay();
                        
			RED_OFF;                 //Display green color
                        GREEN_ON;
                        BLUE_OFF;
                        delay();
                        
			RED_OFF;                 //Display blue color
                        GREEN_OFF;
                        BLUE_ON;
                        delay();
                        
			RED_OFF;                 //Display violet color
                        GREEN_ON;
                        BLUE_ON;
                        delay();
                  }
                }
}

/***********************************************************************************************
*
* @brief    Enable_Interrupt(UINT8 vector_number). Enable interrupts from desired module.
* @param    Module interrupt number from the interrupts vector table
* @return   none
*
************************************************************************************************/
void Enable_Interrupt(UINT8 vector_number)
{

	vector_number= vector_number-16;


	/* Set the ICPR and ISER registers accordingly */
	NVIC_ICPR |= 1 << (vector_number%32);
	NVIC_ISER |= 1 << (vector_number%32);

}
