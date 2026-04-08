/*****************************************************************************
 *   emactest.c:  main C entry file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 ******************************************************************************/
#include <string.h>
#include <nxp/iolpc2378.h>
#include "board.h"
#include "type.h"
#include "irq.h"
#include "target.h"
#include "crc32.h"
#include "emac.h"

#define TX_PACKET_SIZE    114

extern void Reset_Handler( void );
extern volatile DWORD ReceiveLength;
extern volatile DWORD PacketReceived;

#if ENABLE_WOL
extern volatile DWORD WOLArrived;
#endif

/******************************************************************************
** Function name:   PacketGen
**
** Descriptions:    Create a perfect packet for TX
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void PacketGen( BYTE *txptr )
{
int i;
DWORD crcValue;
DWORD BodyLength = TX_PACKET_SIZE - 14;

  /* Dest address */
  *(txptr+0) = EMAC_DST_ADDR56 & 0xFF;
  *(txptr+1) = (EMAC_DST_ADDR56 >> 0x08) & 0xFF;
  *(txptr+2) = EMAC_DST_ADDR34 & 0xFF;
  *(txptr+3) = (EMAC_DST_ADDR34 >> 0x08) & 0xFF;
  *(txptr+4) = EMAC_DST_ADDR12 & 0xFF;
  *(txptr+5) = (EMAC_DST_ADDR12 >> 0x08) & 0xFF;

  /* Src address */
  *(txptr+6) = EMAC_ADDR56 & 0xFF;
  *(txptr+7) = (EMAC_ADDR56 >> 0x08) & 0xFF;
  *(txptr+8) = EMAC_ADDR34 & 0xFF;
  *(txptr+9) = (EMAC_ADDR34 >> 0x08) & 0xFF;
  *(txptr+10) = EMAC_ADDR12 & 0xFF;
  *(txptr+11) = (EMAC_ADDR12 >> 0x08) & 0xFF;

  /* Type or length, body length is TX_PACKET_SIZE - 14 bytes */
  *(txptr+12) = (BodyLength >> 0x08) & 0xFF;
  *(txptr+13) = BodyLength & 0xFF;

  /* Skip the first 14 bytes for dst, src, and type/length */
  for ( i=0; i < BodyLength; i++ )
  {
    *(txptr+i+14) = 0x55;
  }
  crcValue = crc32_bfr( txptr, TX_PACKET_SIZE );

  *(txptr+TX_PACKET_SIZE) = (0xff & crcValue);
  *(txptr+TX_PACKET_SIZE+1) = 0xff & (crcValue >> 8 );
  *(txptr+TX_PACKET_SIZE+2) = 0xff & (crcValue >> 16);
  *(txptr+TX_PACKET_SIZE+3) = 0xff & (crcValue >> 24);
  return;
}

/*****************************************************************************
** Function name:   LED_Blink
**
** Descriptions:    Toogle USB Link LED
**            This is used for WOL test only.
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
void LED_Blink(char state)
{
  if(state)
  {
    USB_LINK_LED_FCLR = USB_LINK_LED_MASK;  /* turn on USB Link LED */
  }
  else
  {
    USB_LINK_LED_FSET = USB_LINK_LED_MASK;  /* turn off USB Link LED */
  }
}

/*****************************************************************************
** Function name:   AllPacketGen
**
** Descriptions:    Fill all the TX buffers based on the number
**            of TX blocks.
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
void AllPacketGen ( void )
{
DWORD i;
BYTE *txptr;

  txptr = (BYTE *)EMAC_TX_BUFFER_ADDR;
  for ( i = 0; i < EMAC_TX_BLOCK_NUM; i++ )
  {
    PacketGen( txptr );
    txptr += EMAC_BLOCK_SIZE;
  }
  return;
}

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
BYTE *txptr;
DWORD i = 0;
#if TX_ONLY
DWORD j;
#endif
#if BOUNCE_RX
BYTE *rxptr;
#endif
#if ENABLE_HASH
long long dstAddr;
DWORD gHashCrcValue;
#endif
#if ENABLE_WOL
DWORD FirstTime_WOL = TRUE;
#endif

  TargetResetInit();

  // USB Link LED port init
  SCS_bit.GPIOM = 1;                      /* enable fast io for GPIO0,1 */
  USB_LINK_LED_FDIR = USB_LINK_LED_MASK;  /* USB Link LED port output */
  USB_LINK_LED_FCLR = USB_LINK_LED_MASK;  /* turn on USB Link LED */

  /********* The main Function is an endless loop ***********/
  while(!EMACInit());

#if TX_ONLY
  /* pre-format the transmit packets */
  AllPacketGen();
#endif

#if ENABLE_HASH
  dstAddr = 0x010203040506;
  gHashCrcValue = do_crc_behav( dstAddr );
  Set_HashValue( gHashCrcValue );
#endif

  EMAC_RxEnable();
  EMAC_TxEnable();

#if BOUNCE_RX
  /* copy just received data from RX buffer to TX buffer and send out */
  txptr = (BYTE *)EMAC_TX_BUFFER_ADDR;
  rxptr = (BYTE *)EMAC_RX_BUFFER_ADDR;

#if ENABLE_WOL
  INTWAKE = 0x10;       /* Ethernet(WOL) Wakeup from powerdown mode */

  LED_Blink(0);          /* Indicating system is in power down now. */
  PCON = 0x02;          /* Power down first */
#endif                  /* endif ENABLE_WOL */

  while( 1 )
  {
#if ENABLE_WOL
    if ( (WOLArrived == TRUE) && (FirstTime_WOL == TRUE) )  /* WOL interrupt has occured */
    {
      WOLArrived = FALSE;
      FirstTime_WOL = FALSE;
      EMAC_RxDisable();
      EMAC_TxDisable();
      /* From power down to WOL, the PLL needs to be reconfigured,
      otherwise, the CCLK will be generated from 4Mhz IRC instead
      of main OSC 12Mhz */
      ConfigurePLL();
      LED_Blink(1);    /* indicating system is awake now. */

      /* Calling EMACInit() is overkill which also initializes the PHY, the
      main reason to do that is to make sure the descriptors and descriptor
      status for both TX and RX are clean and ready to use. It won't go wrong. */
      EMACInit();
      RXFILTERWOLCLEAR = 0xFFFF;  /* set all bits to clear receive filter WOLs */
      RXFILTERCTRL &= ~0x2000;  /* enable Rx Magic Packet and RxFilter Enable WOL */
      INTENABLE = 0x00FF;     /* Enable all interrupts except SOFTINT and WOL */
      EMAC_RxEnable();
      EMAC_TxEnable();
      PacketReceived = FALSE;
    }
#endif                    /* endif ENABLE_WOL */

    if ( PacketReceived == TRUE )
    {
      PacketReceived = FALSE;
      /* Reverse Source and Destination, then copy the body */
      memcpy( (BYTE *)txptr, (BYTE *)(rxptr+6), 6);
      memcpy( (BYTE *)(txptr+6), (BYTE *)rxptr, 6);
      memcpy( (BYTE *)(txptr+12), (BYTE *)(rxptr+12), ReceiveLength );
      EMACSend( (DWORD *)txptr, ReceiveLength - 2 );
      txptr += EMAC_BLOCK_SIZE;
      rxptr += EMAC_BLOCK_SIZE;
      i++;
      /* EMAC_TX_BLOCK_NUM and EMAC_RX_BLOCK_NUM should be the same */
      if ( i == EMAC_TX_BLOCK_NUM )
      {
        i = 0;
        txptr = (BYTE *)EMAC_TX_BUFFER_ADDR;
        rxptr = (BYTE *)EMAC_RX_BUFFER_ADDR;
      }
    }
  }
#endif                    /* endif BOUNCE_RX */

#if TX_ONLY
  /* Transmit packets only */
  while ( 1 )
  {
    txptr = (BYTE *)EMAC_TX_BUFFER_ADDR;
    for ( i = 0; i < EMAC_TX_BLOCK_NUM; i++ )
    {
      EMACSend( (DWORD *)txptr, TX_PACKET_SIZE + 4 );
      txptr += EMAC_BLOCK_SIZE;
      for ( j = 0; j < 0x200000; j++ ); /* delay */
    }
  }
#endif                    /* endif TX_ONLY */

  return 0;
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
