/*****************************************************************************
 *   i2s.c:  I2S C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.19  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "i2s.h"
#include "intrinsics.h"

volatile BYTE I2STXBuffer[BUFSIZE];
volatile BYTE I2SRXBuffer[BUFSIZE];
volatile DWORD I2SReadLength = 0;
volatile DWORD I2SWriteLength = 0;
volatile DWORD I2SRXDone = 0, I2STXDone = 0;

/*****************************************************************************
** Function name:   I2SHandler
**
** Descriptions:    I2S interrupt handler, only RX interrupt is enabled
**            for simplicity.
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
__irq __nested __arm void I2SHandler (void)
{
  DWORD RxCount = 0;

  __enable_interrupt(); /* handles nested interrupt */
  if ( I2SSTATE & 0x01 )
  {
    RxCount = (I2SSTATE >> 8) & 0xFF;
    if ( (RxCount != RXFIFO_EMPTY) && !I2SRXDone )
    {
      while ( RxCount > 0 )
      {
        if ( I2SReadLength == BUFSIZE )
        {
          /* Stop RX channel */
          I2SDAI |= ((0x01 << 3) | (0x01 << 4));
          I2SIRQ &= ~(0x01 << 0); /* Disable RX */
          I2SRXDone = 1;
          break;
        }
        else
        {
          I2SRXBuffer[I2SReadLength++] = I2SRXFIFO;
        }
        RxCount--;
      }
    }
  }

  VICADDRESS = 0;   /* Acknowledge Interrupt */
}

/*****************************************************************************
** Function name:   I2SInit
**
** Descriptions:    Initialize I2S controller
**
** parameters:      None
** Returned value:    true or false, return false if the I2S
**            interrupt handler was not installed correctly
**
*****************************************************************************/
DWORD I2SInit( void )
{
  DWORD DAIValue, DAOValue;

  /*enable I2S in the PCONP register. I2S is disabled on reset*/
  PCONP |= (1 << 27);

  /*connect the I2S sigals to port pins
    P0.6 - RX_SDA, P0.7 - TX_CLK, P0.8 - TX_WS, P0.9 - TX_SDA,
    P0.23 - RX_CLK, P0.24 - RX_WS */

  PINSEL0_bit.P0_6  = 1;
  PINSEL0_bit.P0_7  = 1;
  PINSEL0_bit.P0_8  = 1;
  PINSEL0_bit.P0_9  = 1;
  PINSEL1_bit.P0_23 = 2;
  PINSEL1_bit.P0_24 = 2;

  /* For all the test program assuming USB is used, the CCLK is set to
  57.6Mhz while all the PCLK are the same, 1/2 of CCLK = 28.8Mhz. It also
  applys to I2S. To generate 48khz sample rate for 16-bit stereo data
  requires a bit rate of 48,000x16x2=1.536Mhz, the TX and RX clock rate
  register should be 28800000/1536000=18.75=0x13.

  Please note, in order to generate accurate TX/RX clock rate for I2S,
  PCLK and CCLK needs to be carefully reconsidered. For this test
  program, the TX is looped back to RX without external I2S device,
  clock rate is not critical in this matter. */
  I2STXRATE = 0x13;
  I2SRXRATE = 0x13;

  /* Stop the I2S to start. Audio output is master, audio input is the slave. */
  /* 16 bit data, set STOP and RESET bits to reset the channels */
  DAOValue = I2SDAO;
  /* Switch to master mode, TX channel, no mute */
  DAOValue &= ~((0x01 << 5)|(0x01 << 15));
  DAIValue = I2SDAI;
  DAIValue &= ~(0x01 << 15);
  I2SDAO = (0x01 << 4) | (0x01 << 3) | DAOValue;  /* Master */
  I2SDAI = (0x01 << 4) | (0x01 << 3) | DAIValue; /* Slave */

  if ( install_irq( I2S_INT, (void *)I2SHandler, HIGHEST_PRIORITY ) == FALSE )
  {
    return (FALSE);
  }

  /* Audio output is the master, audio input is the slave, */
  /* 16 bit data, stereo, reset, master mode, not mote. */
  DAOValue = I2SDAO;
  DAIValue = I2SDAI;
  I2SDAO = DAOValue & (~((0x01 << 4)|(0x01 <<3)));
  /* 16 bit data, stereo, reset, slave mode, not mote. */
  I2SDAI  = DAIValue & (~((0x01 << 4)|(0x01 <<3)));;

  /* RX FIFO depth is 1, TX FIFO depth is 8. */
  I2SIRQ = (8 << 16) | (1 << 8) | (0x01 << 0);
  return( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

