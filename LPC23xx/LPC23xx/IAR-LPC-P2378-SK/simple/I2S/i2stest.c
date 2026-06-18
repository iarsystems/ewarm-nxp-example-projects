/*****************************************************************************
 *   i2stest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 ******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "i2s.h"
#include "target.h"

extern volatile BYTE I2STXBuffer[BUFSIZE];
extern volatile BYTE I2SRXBuffer[BUFSIZE];
extern volatile DWORD I2SReadLength;
extern volatile DWORD I2SWriteLength;
extern volatile DWORD I2SRXDone, I2STXDone;

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
DWORD i;

  TargetResetInit();

  /* The test program is to connect CLK, WS, and SDA lines between the
  TX and RX. DAO is the master output, DAI is the slave input. This test
  is not a real-world application but test both TX and RX channel in
  a single program. However, due to the sync issue, the TX will start with
  MUTE data 0 once it starts while RX is not ready. So, it may begins
  with some MUTE value in the beginning of the RX buffer. This is beyond
  control unless throw away the very first few bytes in the RX buffer.
  In this program, the very byte is not compared between the TX buffer and
  RX buffer due to this synchonization issue. */

  /* Configure temp register before reading */
  for ( i = 0; i < BUFSIZE; i++ ) /* clear buffer */
  {
    I2STXBuffer[i] = i;
    I2SRXBuffer[i] = 0;
  }

  if ( I2SInit() == FALSE ) /* initialize I2S */
  {
    while ( 1 );      /* Fatal error */
  }

  while ( I2SWriteLength < BUFSIZE )
  {
    while (((I2SSTATE >> 16) & 0xFF) == TXFIFO_FULL);
    I2STXFIFO = I2STXBuffer[I2SWriteLength++];
  }

  I2STXDone = 1;
  /* Wait for RX and TX complete before comparison */
  while ( !I2SRXDone || !I2STXDone );

  for ( i=1; i<BUFSIZE; i++ )
  {
    if ( I2SRXBuffer[i] != I2STXBuffer[i-1] )
    {
      while ( 1 );  /* Validation error */
    }
  }
  return 0;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
