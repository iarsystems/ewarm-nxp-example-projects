/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : llio.c
 *    Description : Low level IO initialization
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
#include "llio.h"

static volatile LPUART_MemMapPtr LPUART = 0;

/*************************************************************************
 * Function Name: LLIO_Init
 * Parameters: LPUART_MemMapPtr LPUART_Base
 *
 * Return: void
 *
 * Description: LPUART instance configuration for low level IO functions.
 *************************************************************************/
void LLIO_Init(LPUART_MemMapPtr LPUART_Base)
{
  LPUART = LPUART_Base;
}

/*************************************************************************
 * Function Name: MyLowLevelGetchar
 * Parameters: none
 *
 * Return: int
 *
 * Description: Simple low level character input with echo.
 *              Blocking variant. No error flags handling is done.
 *************************************************************************/
static int MyLowLevelGetchar(void)
{
int ch;

  /* Read until there is valid read data */
  do {
    ch = LPUART->DATA;
  } while(ch & LPUART_DATA_RXEMPT_MASK);
  /* Echo character back */
  while(!(LPUART->STAT & LPUART_STAT_TDRE_MASK));
  LPUART->DATA = ch & 0x000000FF;
  return ch & 0x000000FF;
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

  /* If LPUART is not set, return failure */
  if (0 == LPUART)
  {
    return _LLIO_ERROR;
  }

  for (/*Empty */; Bufsize > 0; --Bufsize)
  {
    while(!(LPUART->STAT & LPUART_STAT_TDRE_MASK));  //Wait
    LPUART->DATA = *Buf++ & 0x000000FF;
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

  /* If LPUART is not set, return failure */
  if (0 == LPUART)
  {
    return _LLIO_ERROR;
  }

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
