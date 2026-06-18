/*
 * File:        serial.c
 * Purpose:     Provide common UART routines for serial IO
 *
 * Notes:       
 *              
 */

#include "serial.h"

/********************************************************************/
/*
 * Initialize the UART for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * Parameters:
 *  sysclk      UART module Clock in kHz(used to calculate baud)
 *  baud        UART baud rate
 */
void uart_init(unsigned long int clkspeed, unsigned long int baud)
{     
  uint16 sbr, brfa;
        
  /* Enable the pins for the selected UART */
  if(UART_PORT==UART0) //UART0
  {
    CCM->CCGR0 |= CCM_CCGR0_CG7(1);
    IOMUXC->SINGLE.PTB10 = 0x001011A2;  
    IOMUXC->SINGLE.PTB11 = 0x001011A1;  
  }

  if(UART_PORT==UART1) //UART1
  { 
    CCM->CCGR0 |= CCM_CCGR0_CG8(1);    
    IOMUXC->SINGLE.PTB4 = 0x002011A2;
    IOMUXC->SINGLE.PTB5 = 0x002011A1;      
  }
          
  if(UART_PORT==UART2) //UART2
  {  
    CCM->CCGR0 |= CCM_CCGR0_CG9(1);    
    IOMUXC->SINGLE.PTB6 = 0x007011A2;
    IOMUXC->SINGLE.PTB7 = 0x007011A1;     
  }
       
  UART_PORT->MODEM=0; //Need to clear MODEM register in case BootROM sets it  
  
  /* Make sure that the transmitter and receiver are disabled while we 
   * change settings->
   */
  UART_PORT->C2 &= ~UART_C2_RE_MASK;
  UART_PORT->C2 &= ~UART_C2_TE_MASK;
        
  /* Configure the UART for 8-bit mode, no parity */
  UART_PORT->C1 = 0x00;  /* We need all default settings, so entire register is cleared */
        
  /* Calculate baud settings */
  sbr = (uint16)((clkspeed*1000)/(baud * 16));
                
  UART_PORT->BDH |= UART_BDH_SBR((sbr & 0x1F00) >> 8);
  UART_PORT->BDL = UART_BDL_SBR((uint8)(sbr & 0x00FF));  
            
  /* Determine if a fractional divider is needed to get closer to the baud rate */
  brfa = (((clkspeed*32000)/(baud * 16)) - (sbr * 32));
            
  UART_PORT->C4 &= ~UART_C4_BRFA_MASK;
  UART_PORT->C4 |= UART_C4_BRFA(brfa);
          
  /* Enable receiver and transmitter */
  UART_PORT->C2 |= UART_C2_RE_MASK;
  UART_PORT->C2 |= UART_C2_TE_MASK;       
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified UART
 *
 * Parameters:
 *  channel      UART channel to read from
 *
 * Return Values:
 *  the received character
 */
unsigned char uart_getchar()
{ 
  while(!(UART_PORT->S1 & UART_S1_RDRF_MASK))
  { }
            
  /* Return the 8-bit data from the receiver */
  return(UART_PORT->D);
        
}
/********************************************************************/
/*
 * Wait for space in the UART Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      UART channel to send to
 *  ch			 character to send
 */ 
void uart_putchar (char ch)
{
  /* Wait until space is available in the FIFO */
  while(!(UART_PORT->S1 & UART_S1_TDRE_MASK))
  {
  }
          
  /* Send the character */
  UART_PORT->D = (uint8)ch;  
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      UART channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart_getchar_present ()
{
  return (UART_PORT->S1 & UART_S1_RDRF_MASK); 
}
/********************************************************************/
    
