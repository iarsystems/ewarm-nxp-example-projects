/*
 * KBI.c
 *
 *  Created on: Sep 26, 2013
 *      Author: B46911
 */

#include "KBI.h"
#include "GPIO.h"

/**********************************************************************************************
* External objects
**********************************************************************************************/


/**********************************************************************************************
* Global variables
**********************************************************************************************/
pt2FuncU8 KBI_Callback;

/**********************************************************************************************
* Constants and macros
**********************************************************************************************/


/**********************************************************************************************
* Local types
**********************************************************************************************/


/**********************************************************************************************
* Local function prototypes
*********************************************************************************************/


/**********************************************************************************************
* Local variables
**********************************************************************************************/


/**********************************************************************************************
* Local functions
**********************************************************************************************/


/**********************************************************************************************
* Global functions
**********************************************************************************************/
extern void delay();

/***********************************************************************************************
*
* @brief    KBI_Init - Enable Keyboard interrupts on channels 4 and 5 (SW1 and SW2)
* @param    none
* @return   none
*
************************************************************************************************/
void KBI_Init()
{

	SIM_SCGC |=  SIM_SCGC_KBI1_MASK; /* Enable bus clock on KBI1 */
	KBI1_SC = 0;  				  /* Clearing mechanism */
//	KBI1_ES |= KBI_ES_KBEDG(0x0000010);    /* Polarity setting, falling edge low level ,SW1 */
//	KBI1_ES |= KBI_ES_KBEDG(0x0000020);    /* Polarity setting,falling edge low level,SW2 */
	KBI1_ES |= KBI_ES_KBEDG(0x000000);    /* Polarity setting, falling edge low level ,SW1 */
	KBI1_ES |= KBI_ES_KBEDG(0x000000);    /* Polarity setting,falling edge low level,SW2 */
	KBI1_PE |= KBI_PE_KBIPE(0x0000010);   /* Enable KBI1 channel 4 , SW1 */
	KBI1_PE |= KBI_PE_KBIPE(0x0000020);   /* Enable KBI1 channel 5 , SW2 */
	KBI1_SC = 0;  				  		/* Clearing flags*/
	KBI1_SC|=KBI_SC_RSTKBSP_MASK;		/* Clearing RSTKBSP bit*/
	KBI1_SC |= KBI_SC_KBIE_MASK;  /* Enable  KBI1 Interrupts */

}
/***********************************************************************************************
*
* @brief    KBI_SetCallback - Set a callback function to execute on a falling edge of an enabled KBI pin
* @param    pointer to function with an UINT8 argument
* @return   none
*
************************************************************************************************/

void KBI_SetCallback(pt2FuncU8 ptr)
{
	KBI_Callback = ptr;
}


/***********************************************************************************************
*
* @brief    KBI1_IRQHandler - KBI interrupt routine, calls the user callabck
* @param    none
* @return   none
*
************************************************************************************************/
void KBI1_IRQHandler()

{

	KBI1_SC |= KBI_SC_KBACK_MASK;	/*Clear flag*/

        delay();        //It's remarkably hard to press both buttons at *precisely* the same time, so
                        //we do a little delay function before we check to see which buttons are pushed.
        
        if(!READ_INPUT(PTE,PTE4) && !READ_INPUT(PTE,PTE5))
	{//If both buttons are pressed, cycle through colors
		KBI_Callback(0);

	}
        if(READ_INPUT(PTE,PTE4))
	{//If button one is pressed, do the flashlight app! (white light)
		KBI_Callback(1);

	}
        else if(READ_INPUT(PTE,PTE5))
	{//If button two is pressed, turn light off
		KBI_Callback(2);

	}
          
//        volatile int temp = KBI_SP_SP(0x0000010);
//
//	if((KBI1_SP & KBI_SP_SP(0x0000010))>>4) /* If SW1 has been pressed */
//	{
//		KBI_Callback(0);
//
//	}
//
//	else if((KBI1_SP & KBI_SP_SP(0x0000020))>>5) /* If SW2 has been pressed */
//	{
//		KBI_Callback(1);
//
//	}


}

