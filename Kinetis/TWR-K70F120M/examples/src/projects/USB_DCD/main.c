/*
 * File:		USB_device.c
 * Purpose:		Main process
 *
 */
#include "common.h"     /* Common definitions */
#include "USB_CDC.h"    /* USB DCD support */
#include "USB_Reg.h"    /* USB regulator */
#include "USB_DCD.h"    /* DCD support */
#include "Dual_Role/psptypes.h"
#include "Dual_Role/MAX3353.h"
#include "Dual_Role/Settings.h"
#include "Dual_Role/i2c_1.h"

#define ENABLE_USB_5V   GPIOB_PSOR|=(1<<8);
#define DISABLE_USB_5V  GPIOB_PCOR|=(1<<8);


/* Extern Variables */
extern UINT8 gu8USB_Flags;
extern UINT8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];
extern uint_8 gu8MAX3353StatusRegister;

/* Global Variales */
uint_8  gu8ISR_Flags=0;



/********************************************************************/
void PortB_ISR(void)
{
    uint_32 u32Value;
    u32Value = PORTB_ISFR;
    PORTB_ISFR = u32Value;
    MAX3353_ISR();
}

/********************************************************************/
void port_config (void)
{

    /* Turn on all port clocks */
    SIM_SCGC5|=  SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;

    /* IRQ_3b and GPIO4 pin configuration*/
    PORTB_PCR8=(0|PORT_PCR_MUX(1));         // configure pin as GPIO
    GPIOB_PDDR|=(1<<8);                     // set as output

    enable_irq(INT_PORTB-16);
    PORTB_PCR6=(0|PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK|PORT_PCR_IRQC(0x0A));

}



/********************************************************************/
void main (void)
{
    //EnableInterrupts;

    // Making sure tat Regulator is enable on every low power mode
    USB_REG_SET_ENABLE;
    USB_REG_SET_STDBY_STOP;
    USB_REG_SET_STDBY_VLPx;

    port_config();
    init_I2C();

    DISABLE_USB_5V;                     // Disable USB 5v (for Host support)

    if(MAX3353_Init())
        printf("\nMAX3353 not Present\n");
    else
        printf("\nMAX3353 Present\n");


    CDC_Init();     // USB CDC Initialization
    DCD_Init();     // Enabling DCD

    /* Here we activate te DCD start sequence. This section must be triggered
      with the VBUs detection pin, but in this case will work once after
      the board is powered in the Kinetis USB connector */

    printf("\n********** DCD/USB Device Module Testing **********\n");

    while(1)
    {
        // USB CDC service routine
        CDC_Engine();

        // DCD detection service
        (void)DCD_Service();

        // MAX3353 service
        if(FLAG_CHK(_MAX3353,gu8ISR_Flags))
        {
            if(gu8MAX3353StatusRegister & VBUS_HIGH_EVENT)
            {
                USBDCD_CONTROL=USBDCD_CONTROL_IE_MASK  | USBDCD_CONTROL_IACK_MASK;
                FLAG_SET(USBDCD_CONTROL_START_SHIFT,USBDCD_CONTROL);
                printf("\nVBUS is Up");
            }
            if(gu8MAX3353StatusRegister & VBUS_LOW_EVENT)
            {
                USB_DISABLE_PULLUP;
                printf("\nVBUS is Down");
            }
            FLAG_CLR(_MAX3353,gu8ISR_Flags);
        }

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
