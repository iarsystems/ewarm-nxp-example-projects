/**
  \mainpage 
  \n Copyright (c) 2011 Freescale
  \n Freescale Confidential Proprietary 
  \brief 
  \author   	Freescale Semiconductor
  \author    
  \version      m.n
  \date         Aug 06, 2012
  
  Put here all the information needed of the Project. Basic configuration as well as information on
  the project definition will be very useful 
*/
/****************************************************************************************************/
/*                                                                                                  */
/* All software, source code, included documentation, and any implied know-how are property of      */
/* Freescale Semiconductor and therefore considered CONFIDENTIAL INFORMATION.                       */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.                      */
/*                                                                                                  */
/* All Confidential Information remains the property of Freescale Semiconductor and will not be     */
/* copied or reproduced without the express written permission of the Discloser, except for copies  */
/* that are absolutely necessary in order to fulfill the Purpose.                                   */
/*                                                                                                  */
/* Services performed by FREESCALE in this matter are performed AS IS and without any warranty.     */
/* CUSTOMER retains the final decision relative to the total design and functionality of the end    */
/* product.                                                                                         */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the success of this project.*/
/*                                                                                                  */
/* FREESCALE disclaims all warranties, express, implied or statutory including, but not limited to, */
/* implied warranty of merchantability or fitness for a particular purpose on any hardware,         */
/* software ore advise supplied to the project by FREESCALE, and or any product resulting from      */
/* FREESCALE services.                                                                              */
/* In no event shall FREESCALE be liable for incidental or consequential damages arising out of     */
/* this agreement. CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or */
/* actions by anyone on account of any damage,or injury, whether commercial, contractual, or        */
/* tortuous, rising directly or indirectly as a result of the advise or assistance supplied CUSTOMER*/ 
/* in connectionwith product, services or goods supplied under this Agreement.                      */
/****************************************************************************************************/


/****************************************************************************************************/
/* Basic function the LPTMR will wake up the MCU every 100ms depending on the selected application  */ 
/* the Potentiometer or the Accelerometer will be read.                                             */                 
/*                                                                                                  */
/* In the case of the potentiometer the MCU must go back to sleep if the read voltage is under 2v.  */
/* For the accelerometer the MCU goes back to sleep while the tower board is on vertical position   */
/* (normal tower position). If those conditions are not fulfilled a message must be send thru  UART */
/* every second and LED D8 on the tower must be toggling.                                           */
/*                                                                                                  */ 
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "common.h"

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/

#include "common.h"
#include "uart.h"
#include "LowPowerAppStates.h"

/**
 * \brief   main
 * \author  
 * \param  none
 * \return none
 * \todo
 * \warning
 */  
void main (void)
{
	uint8 u8AppSelect;

        CLK_OUT_DISABLE;                //Disable Clock out to reduce consumption
    
	vfnMCGandUARTConfig();
        vfnEnableClockGates();          
	vfnLowPowerConfig();            
	vfnLEDsOutPutConfig();
		
	printf("\nPlease Select the number for the component application you want to run. Use reset button to back this menu\n");
	printf(" 1. Accelerometer\n");
	printf("	/**********************************************************************************/\n");
	printf("	/ In this application the MCU will be sleep while the board is on vertical position/\n");
	printf("	/         if not a message will be send thru UART and LED D8 will toggle           /\n");
	printf("	/**********************************************************************************/\n");
	printf(" 2. Potentiometer\n");
	printf("	/**********************************************************************************/\n");
	printf("	/ In this application the MCU will be sleep while the Potentiometer will be under  /\n");
	printf("	/         2v if not a message will be send thru UART and LED D8 will toggle        /\n");
	printf("	/**********************************************************************************/\n");
	
        u8AppSelect = uart_getchar(TERM_PORT);		
	if(u8AppSelect == '1')
	{
            gu8AppSelected = ACC;
	    printf("Accelerometer application starts. Move tower to horizontal position to wake up the MCU\n");
	}else
	{
            gu8AppSelected = POT;
            printf("Potentiometer application starts. Increment potentiometer value to wake up the MCU\n");
	}
	
	
    if(gu8AppSelected)          //Depending on the chosen component configure the control modules
    	vfnAccelConfig();
    else
    	vfnADCConfig();
	
    vfnLPTMRConfig();
    
    sMLowPowerDemoState.ActualState = LP_DEMO_IDLE_STATE;    //Initialize machine state

	while(1)
	{  
           vpfnLowPowerDemoStateMachine[sMLowPowerDemoState.ActualState]();   //Call to machine state        
	} 
}
/********************************************************************/
