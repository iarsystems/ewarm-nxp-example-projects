/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : uart.c
 *    Description : uart module functions
 *
 *    History :
 *    1. Datå        : 14.11.2007
 *       Author      : Stoyan Choynev
 *       Description : initial version
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include <NXP\iolpc2103.h>
#include "main.h"
#include "uart.h"
#include <yfuns.h>

/** local definitions **/
#define UART0_BAUDRATE  115200UL
/** internal functions **/
static int MyLowLevelGetchar(void);
/** public functions **/

/*************************************************************************
 * Function Name: UART0_Init
 * Parameters: None
 *
 * Return: None
 *
 * Description: Initialization of the UART0 module.
 *
 *************************************************************************/

void UART0_Init (void)
{
  PINSEL0_bit.P0_0 = 1;     //Uart TX function select
  PINSEL0_bit.P0_1 = 1;     //Uart RX function select

  U0LCR_bit.WLS = 3;        //Serlect 8-Bits
  U0LCR_bit.SBS = 0;        //One Stop Bit
  U0LCR_bit.PE = 0;         //No parity
  
  U0LCR_bit.DLAB = 1;       //Devisor Latch Access ebabled
  //Set UART0 BAUDRATE
  U0DLL = ((PCLK/16)/UART0_BAUDRATE)%256;  //
  U0DLM = ((PCLK/16)/UART0_BAUDRATE)/256;  //
  U0LCR_bit.DLAB = 0;           //Clear Devisor Latch Access bit
  
  U0FCR = (1<<FIFO_EN);     //Enable UART0 FIFOs.
  U0TER = (1<<TX_EN);       //Enable Transmission
}
/*************************************************************************
 * Function Name: __write
 * Parameters: Low Level cahracter output
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while( !U0LSR_bit.THRE );  //Wait
    U0THR = * Buf++;
    ++nChars;
  }
  return nChars;
}
/*************************************************************************
 * Function Name: __read
 * Parameters: Low Level cahracter input
 *
 * Return:
 *
 * Description:
 *
 *************************************************************************/
size_t __read(int handle, unsigned char * buffer, size_t size)
{
  int nChars = 0;

  /* This template only reads from "standard in", for all other file
   * handles it returns failure. */
  if (handle != _LLIO_STDIN)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size > 0; --size)
  {
    int c = MyLowLevelGetchar();
    if (c < 0)
      break;

    *buffer++ = c;
    ++nChars;
  }

  return nChars;
}
/** private functions **/
static int MyLowLevelGetchar(void)
{
int ch;

  if(U0LSR_bit.DR)
  {
    ch = U0RBR; 
    if(U0LSR_bit.OE || U0LSR_bit.PE || U0LSR_bit.FE )
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
