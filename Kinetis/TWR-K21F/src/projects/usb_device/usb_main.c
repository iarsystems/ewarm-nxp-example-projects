/*
 * File:		usb_main.c
 * Purpose:		Main process
 *
 */

/* Includes */
#include "common.h"
#include "usb_cdc.h"
#include "usb_reg.h"
#include "Settings.h"

/* Extern Variables */
extern uint8 gu8USB_Flags; 
extern uint8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];

/********************************************************************/
void main (void)
{
    printf("\n********** USB Device Module Testing **********\n");
    // disable MPU
    MPU_CESR = 0;                                    
    USB_REG_SET_ENABLE;
    USB_REG_SET_STDBY_STOP;      
    USB_REG_SET_STDBY_VLPx;
    
    // USB CDC Initialization
    CDC_Init();
    
    while(1)
    {
        
       // USB CDC service routine 
          CDC_Engine();

        // If data transfer arrives
        if(FLAG_CHK(EP_OUT,gu8USB_Flags))
        {
            (void)USB_EP_OUT_SizeCheck(EP_OUT);         
            usbEP_Reset(EP_OUT);
            usbSIE_CONTROL(EP_OUT);
            FLAG_CLR(EP_OUT,gu8USB_Flags);

            // Send it back to the PC
            EP_IN_Transfer(EP2,CDC_OUTPointer,1);

        }        
        
    } 
}

/*******************************************************************/

