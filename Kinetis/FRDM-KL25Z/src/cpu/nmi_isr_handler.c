/* File:         nmi_isr_handler.c
 * Purpose:     Provides routines for handling nmi isr.
 *
 * Notes:	
 *              
 */
void nmi_isr(void);

#include "common.h"
#include "nmi_isr_handler.h"



void nmi_isr(void)
{
   return;  
}

void enable_NMI_button(void)
{
    /* Configure the PTA4 pin for its NMI function */
  
  PORTA_PCR4 =  PORT_PCR_PS_MASK |
                PORT_PCR_PE_MASK | 
                PORT_PCR_PFE_MASK |
                PORT_PCR_MUX(7); /* NMI Falling edge */    
   // NMI is alt7 function for this pin
    
}
