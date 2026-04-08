/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : llio.c
 *    Description : Low Level IO redirection driver
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 7084 $
 **************************************************************************/
#include <yfuns.h>
#include "device_registers.h"

static UART_MemMapPtr UART;

static int MyLowLevelGetchar(void);

/*************************************************************************
 * Function Name: LLIO_Init
 * Parameters: UART_MemMapPtr UART_BASE
 *
 * Return: void
 *
 * Description: UART instance configuration for low level IO functions.
 *************************************************************************/
void LLIO_Init(UART_MemMapPtr UART_BASE)
{
  UART = UART_BASE;
}

/*************************************************************************
 * Function Name: __write
 * Parameters: int Handle, const unsigned char * Buf, size_t Bufsize
 *
 * Return: size_t
 *
 * Description: Low Level character output
 *************************************************************************/
size_t __write(int Handle, const unsigned char * Buf, size_t Bufsize)
{
size_t nChars = 0;

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while(UART->UTS & (1<<UART_UTS_TXFULL_SHIFT));
    UART->UTXD = *Buf++;
    ++nChars;
  }
  return nChars;
}

/*************************************************************************
 * Function Name: __read
 * Parameters: int handle, unsigned char * buffer, size_t size
 *
 * Return: size_t
 *
 * Description: Low Level character input
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

/*************************************************************************
 * Function Name: MyLowLevelGetchar
 * Parameters: none
 *
 * Return: int
 *
 * Description: Low Level getchar
 *************************************************************************/
static int MyLowLevelGetchar(void)
{
int ch;

  if(! (UART->UTS & (1<<UART_UTS_RXEMPTY_SHIFT)))
  {
    ch = UART->URXD;
    if(!(ch & (1<<UART_URXD_CHARRDY_SHIFT)) || (ch & (1<<UART_URXD_ERR_SHIFT)))
    {
      return (ch | 0x10000000);
    }
    return (ch & 0xff );
  }
  return -1;
}
