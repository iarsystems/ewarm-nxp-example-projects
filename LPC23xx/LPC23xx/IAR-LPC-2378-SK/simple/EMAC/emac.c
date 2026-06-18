/*****************************************************************************
 *   emac.c:  Ethernet module file for NXP LPC230x Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <nxp/iolpc2378.h>
#include "type.h"
#include "irq.h"
#include "emac.h"

#include <intrinsics.h>

volatile DWORD Duplex;
volatile DWORD Speed;

volatile DWORD RXOverrunCount = 0;
volatile DWORD RXErrorCount = 0;

volatile DWORD TXUnderrunCount = 0;
volatile DWORD TXErrorCount = 0;
volatile DWORD RxFinishedCount = 0;
volatile DWORD TxFinishedCount = 0;
volatile DWORD TxDoneCount = 0;
volatile DWORD RxDoneCount = 0;

volatile DWORD CurrentRxPtr = EMAC_RX_BUFFER_ADDR;
volatile DWORD ReceiveLength = 0;
volatile DWORD PacketReceived = FALSE;

#if ENABLE_WOL
volatile DWORD WOLCount = 0;
volatile DWORD WOLArrived = FALSE;
#endif

static DWORD PhyAddr;

/******************************************************************************
** Function name:   EMAC_TxEnable/EMAC_TxDisable
**
** Descriptions:    EMAC TX API modules
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void EMAC_TxEnable( void )
{
  COMMAND |= 0x02;
  return;
}

void EMAC_TxDisable( void )
{
  COMMAND &= ~0x02;
  return;
}

/******************************************************************************
** Function name:   EMAC_RxEnable/EMAC_RxDisable
**
** Descriptions:    EMAC RX API modules
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
void EMAC_RxEnable( void )
{
  COMMAND |= 0x01;
  MAC1 |= 0x01;
  return;
}

void EMAC_RxDisable( void )
{
  COMMAND &= ~0x01;
  MAC1 &= ~0x01;
  return;
}

/******************************************************************************
** Function name:   EMACHandler
**
** Descriptions:    EMAC interrupt handler
**
** parameters:      None
** Returned value:    None
**
******************************************************************************/
__irq __nested __arm void EMACHandler (void)
{
volatile DWORD IntStatus;
DWORD *rxptr;

  __enable_interrupt(); /* handles nested interrupt */

  IntStatus = INTSTATUS;
  if ( IntStatus != 0 ) /* At least one interrupt */
  {
#if ENABLE_WOL
    if ( IntStatus & EMAC_INT_WOL )
    {
      INTCLEAR = EMAC_INT_WOL;
      WOLCount++;
      WOLArrived = TRUE;
      /* the packet will be lost, no need to anything else, bail out */
      VICAddress = 0;    /* Acknowledge Interrupt */
      return;
    }
#endif
    if ( IntStatus & EMAC_INT_RXOVERRUN )
    {
      INTCLEAR = EMAC_INT_RXOVERRUN;
      RXOverrunCount++;
      VICADDRESS = 0; /* Acknowledge Interrupt */
      return;
    }

    if ( IntStatus & EMAC_INT_RXERROR )
    {
      INTCLEAR = EMAC_INT_RXERROR;
      RXErrorCount++;
      VICADDRESS = 0; /* Acknowledge Interrupt */
      return;
    }

    if ( IntStatus & EMAC_INT_RXFINISHED )
    {
      INTCLEAR = EMAC_INT_RXFINISHED;
      RxFinishedCount++;
      /* Below should never happen or RX is seriously wrong */
      while ( RXPRODUCEINDEX != (RXCONSUMEINDEX - 1) );
    }

    if ( IntStatus & EMAC_INT_RXDONE )
    {
      INTCLEAR = EMAC_INT_RXDONE;
      ReceiveLength = EMACReceive( rxptr );
      PacketReceived = TRUE;
      RxDoneCount++;
    }

    if ( IntStatus & EMAC_INT_TXUNDERRUN )
    {
      INTCLEAR = EMAC_INT_TXUNDERRUN;
      TXUnderrunCount++;
      VICADDRESS = 0; /* Acknowledge Interrupt */
      return;
    }

    if ( IntStatus & EMAC_INT_TXERROR )
    {
      INTCLEAR = EMAC_INT_TXERROR;
      TXErrorCount++;
      VICADDRESS = 0; /* Acknowledge Interrupt */
      return;
    }

    if ( IntStatus & EMAC_INT_TXFINISHED )
    {
      INTCLEAR = EMAC_INT_TXFINISHED;
      TxFinishedCount++;
    }

    if ( IntStatus & EMAC_INT_TXDONE )
    {
      INTCLEAR = EMAC_INT_TXDONE;
      TxDoneCount++;
    }
  }

  VICADDRESS = 0;                   /* Acknowledge Interrupt */
  return;
}

/*****************************************************************************
** Function name:   WritePHY
**
** Descriptions:    Write Data to the PHY port
**
** parameters:      PHY register, write data
** Returned value:    None
**
*****************************************************************************/
void WritePHY( DWORD PHYReg, DWORD PHYData )
{
  MADR = PhyAddr | PHYReg; /* [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr */
  MCMD = 0x0000;      /* write command */
  MWTD = PHYData;
  while ( MIND_bit.BUSY );
  return;
}

/*****************************************************************************
** Function name:   ReadPHY
**
** Descriptions:    Read data from the PHY port
**
** parameters:      PHY register
** Returned value:    PHY data
**
*****************************************************************************/
DWORD ReadPHY( DWORD PHYReg )
{
  MADR = PhyAddr | PHYReg; /* [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr */
  MCMD = 0x0001;      /* read command */
  while ( MIND_bit.BUSY || MIND_bit.NOT_VALID );
  MCMD = 0x0000;
  return( MRDD );
}

/*****************************************************************************
** Function name:   PHYInit
**
** Descriptions:    initialize PHY port
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
DWORD PHYInit( void )
{
DWORD i, regValue;
DWORD timeout;

#if RMII
  /* RMII configuration */
  MCFG = 0x0018;	/* host clock divided by 20, no suppress preamble, no scan increment */
  COMMAND |= 0x0200;
  SUPP = 0x8900;  /* RMII setting, Reset MII Management hardware,
                     PHY support: [8]=0 ->10 Mbps mode, =1 -> 100 Mbps mode */
  /*  (note bit 4 was set in original test, although spec says its unused) */
  for ( i = 0; i < 0x20; i++ ); /* short delay */
  SUPP = 0x0100;
#else
  COMMAND &= ~0x0200;
  SUPP = 0x0000;
#endif

  /* Find PHY Address, because controller have pull-up and PHY have pull-down resistors
     on RXD lines some times PHY latch different addresses - 0x01, 0x19, 0x09, 0x17 */

  for(PhyAddr = (1UL << 8); PhyAddr < (32UL << 8); PhyAddr += (1UL << 8))
  {
    /* check PHY IDs to make sure the reset takes place and PHY
    is in its default state. See Micrel PHY KS8721 Users Manual
    for more details */
    regValue = ReadPHY( PHY_PHYIDR1 );
    if ( (regValue & 0xFFFF) != 0x0022 )
    {
      continue;
    }

    regValue = ReadPHY( PHY_PHYIDR2 );
    if ( (regValue & 0xFFFF) == 0x1619 )
    {
      break;
    }
  }

  if(PhyAddr == (32UL << 8))
  {
    // Can't find Micrel PHY
    return( FALSE );
  }


  WritePHY( PHY_BMCR, BMCR_RESET );
  for ( i = 0; i < 0x20; i++ ); /* short delay */

  timeout = MII_BMSR_TIMEOUT;
  while ( timeout != 0 )
  {
    regValue = ReadPHY( PHY_BMCR );
    if ( (regValue & BMCR_RESET) == 0x0000 )
    {
      break;    /* Reset self cleaned once the reset process is complete */
    }
    timeout--;
  }
  if ( timeout == 0 )
  {
    return ( FALSE );
  }

  if(ReadPHY( PHY_BMSR ) & BMSR_PREAMBLE_SUPP)
  {
    // PHY support preamble suppression
    MCFG |= 1 << 1;
  }

#if AUTO_NEGOTIATION_ENA != 0
  /* set Auto-Negotiation Advertisement Register
     a valid 802.3 selector field, 100 HD, 100 FD, 10 HD, 10 FD */
  WritePHY( PHY_ANAR, 0x01E1 );

  WritePHY( PHY_BMCR, BMCR_AN | BMCR_RE_AN ); /* auto negotiation, restart AN */
  timeout = MII_BMSR_TIMEOUT;
  while ( timeout != 0 )
  {
    regValue = ReadPHY( PHY_BMSR );
    if ( (regValue & (BMSR_LINK_ESTABLISHED | BMSR_AUTO_DONE)) == (BMSR_LINK_ESTABLISHED | BMSR_AUTO_DONE))
    {
      break;    /* link established if bit 0 is set */
    }
    timeout--;
  }

  if ( timeout == 0 )
  {
    return ( FALSE );
  }

  /* Link established from here on */
  regValue = ReadPHY( PHY_PHYCR );
  switch(regValue & PHYCR_MODE)
  {
  case 0x04:  // 10 BASE T Half-duplex
    Speed = SPEED_10;
    Duplex = HALF_DUPLEX;
    break;
  case 0x08:  // 100 BASE TX Half-duplex
    Speed = SPEED_100;
    Duplex = HALF_DUPLEX;
    break;
  case 0x14: // 10 BASE T Full-duplex
    Speed = SPEED_10;
    Duplex = FULL_DUPLEX;
    break;
  case 0x18: // 100 BASE TX Full-duplex
    Speed = SPEED_100;
    Duplex = FULL_DUPLEX;
    break;
  default:
    return ( FALSE );
  }
#else
  /* Set fixed Speed and Duplex settings */
  WritePHY( PHY_BMCR, ((FIX_SPEED == SPEED_100)?BMCR_SPEED_100:0) |
                      ((FIX_DUPLEX == FULL_DUPLEX)?BMCR_DUPLEX:0));
  timeout = MII_BMSR_TIMEOUT;
  while ( timeout != 0 )
  {
    regValue = ReadPHY( PHY_BMSR );
    if (regValue & BMSR_LINK_ESTABLISHED)
    {
      break;    /* link established if bit 0 is set */
    }
    timeout--;
  }

  if ( timeout == 0 )
  {
    return ( FALSE );
  }

  Speed = FIX_SPEED;
  Duplex = FIX_DUPLEX;
#endif // AUTO_NEGOTIATION_ENA != 0

  return ( TRUE );
}

/*****************************************************************************
** Function name:   EMACTxDesciptorInit
**
** Descriptions:    initialize EMAC TX descriptor table
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
void EMACTxDescriptorInit( void )
{
DWORD i;
DWORD *tx_desc_addr, *tx_status_addr;

  /*-----------------------------------------------------------------------------
  * setup the Tx status,descriptor registers --
  * Note, the actual tx packet data is loaded into the ahb2_sram16k memory as part
  * of the simulation
  *----------------------------------------------------------------------------*/
  TXDESCRIPTOR = TX_DESCRIPTOR_ADDR;  /* Base addr of tx descriptor array */
  TXSTATUS = TX_STATUS_ADDR;    /* Base addr of tx status */
  TXDESCRIPTORNUMBER = EMAC_TX_DESCRIPTOR_COUNT - 1;  /* number of tx descriptors, 16 */

  for ( i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++ )
  {
    tx_desc_addr = (DWORD *)(TX_DESCRIPTOR_ADDR + i * 8); /* two words at a time, packet and control */
    *tx_desc_addr = (DWORD)(EMAC_TX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE);
    *(tx_desc_addr+1) = (DWORD)(EMAC_TX_DESC_INT | (EMAC_BLOCK_SIZE - 1));  /* set size only */
  }

  for ( i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++ )
  {
    tx_status_addr = (DWORD *)(TX_STATUS_ADDR + i * 4); /* TX status, one word only, status info. */
    *tx_status_addr = (DWORD)0;   /* initially, set status info to 0 */
  }
  TXPRODUCEINDEX = 0x0; /* TX descriptors point to zero */
  return;
}

/*****************************************************************************
** Function name:   EMACRxDesciptorInit
**
** Descriptions:    initialize EMAC RX descriptor table
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
void EMACRxDescriptorInit( void )
{
DWORD i;
DWORD *rx_desc_addr, *rx_status_addr;

  /*-----------------------------------------------------------------------------
  * setup the Rx status,descriptor registers --
  * Note, the actual rx packet data is loaded into the ahb2_sram16k memory as part
  * of the simulation
  *----------------------------------------------------------------------------*/
  RXDESCRIPTOR = RX_DESCRIPTOR_ADDR;  /* Base addr of rx descriptor array */
  RXSTATUS = RX_STATUS_ADDR;      /* Base addr of rx status */
  RXDESCRIPTORNUMBER = EMAC_RX_DESCRIPTOR_COUNT - 1;  /* number of rx descriptors, 16 */

  for ( i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++ )
  {
    /* two words at a time, packet and control */
    rx_desc_addr = (DWORD *)(RX_DESCRIPTOR_ADDR + i * 8);
    *rx_desc_addr = (DWORD)(EMAC_RX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE);
    *(rx_desc_addr+1) = (DWORD)(EMAC_RX_DESC_INT | (EMAC_BLOCK_SIZE - 1));  /* set size only */
  }

  for ( i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++ )
  {
    /* RX status, two words, status info. and status hash CRC. */
    rx_status_addr = (DWORD *)(RX_STATUS_ADDR + i * 8);
    *rx_status_addr = (DWORD)0; /* initially, set both status info and hash CRC to 0 */
    *(rx_status_addr+1) = (DWORD)0;
  }
  RXCONSUMEINDEX = 0x0; /* RX descriptor points to zero */
  return;
}

/*****************************************************************************
** Function name:   EMACInit
**
** Descriptions:    initialize EMAC port
**
** parameters:      None
** Returned value:    None
**
*****************************************************************************/
DWORD EMACInit( void )
{
DWORD regVal;
DWORD i;

  /* turn on the ethernet MAC clock in PCONP, bit 30 */
  regVal = PCONP;
  regVal |= PCONP_EMAC_CLOCK;
  PCONP = regVal;

  /*------------------------------------------------------------------------------
  * write to PINSEL2/3 to select the PHY functions on P1[17:0]
  *-----------------------------------------------------------------------------*/
  /* documentation needs to be updated */
#if RMII
  // Pins assignment
  //------------------------------------------------------
  // Write to PINSEL2/3 to select the PHY functions on P1[17:0]
  // P1.6, ENET-TX_CLK, has to be set for Rev '-' devices and it
  // must not be set for Rev 'A’ and newer devices
  //------------------------------------------------------
  regVal = MAC_MODULEID;
  if ( regVal == OLD_EMAC_MODULE_ID )
  {
    // On Rev. '-', MAC_MODULEID should be equal to OLD_EMAC_MODULE_ID, P1.6 should be set.
    PINSEL2 = 0x50151105;
    // selects P1[0,1,4,6,8,9,10,14,15]
  }
  else
  {
    // on rev. 'A', MAC_MODULEID should not equal to OLD_EMAC_MODULE_ID, P1.6 should not be set.
    PINSEL2 = 0x50150105;
    // selects P1[0,1,4,8,9,10,14,15]
  }
  PINSEL3 = 0x00000005; /* selects P1[17:16] */
#else
  PINSEL2 = 0x55555555; /* selects P1[15:0] */
  PINSEL3 = 0x00000005; /* selects P1[17:16] */
#endif

  /*-----------------------------------------------------------------------------
  * write the MAC config registers
  *----------------------------------------------------------------------------*/
  MAC1 = 0xCF00;  /* [15],[14],[11:8] -> soft resets all MAC internal modules */
  COMMAND = 0x0038; /* reset all datapaths and host registers */

  for ( i = 0; i < 0x04; i++ ); /* short delay after reset */
  MAC1 = 0x0;   /* deassert all of the above soft resets in MAC1 */

  EMAC_TxDisable();
  EMAC_RxDisable();

  MAC2 = 0x00;    /* initialize MAC2 register to default value */

  /* Non back to back inter-packet gap */
  IPGR = 0x0012;  /* use the default value recommended in the users manual */

  CLRT = 0x370F;  /* Use the default value in the users manual */
  MAXF = 0x0600;  /* Use the default value in the users manual */

  /* by default, in MAC_COMMAND, bit 9 is set to zero, in MII mode */
  if ( PHYInit() == FALSE )
  {
    return ( FALSE );
  }

  /* write the station address registers */
  SA0 = EMAC_ADDR12;
  SA1 = EMAC_ADDR34;
  SA2 = EMAC_ADDR56;

  if ( (Speed == SPEED_10) && (Duplex == HALF_DUPLEX) )
  {
    MAC2 = 0x30;    /* half duplex, CRC and PAD enabled. */
    SUPP &= ~0x0100;  /* RMII Support Reg. speed is set to 10M */
#if RMII
    COMMAND |= 0x0240;
#else
    COMMAND |= 0x0040;  /* [10]-half duplex,[9]-MII mode,[6]-Pass runt frame,
    [5]-RxReset */
#endif
    /* back to back int-packet gap */
    IPGT = 0x0012;    /* IPG setting in half duplex mode */
  }
  else if ( (Speed == SPEED_100) && (Duplex == HALF_DUPLEX) )
  {
    MAC2 = 0x30;    /* half duplex, CRC and PAD enabled. */
    SUPP |= 0x0100;   /* RMII Support Reg. speed is set to 100M */
#if RMII
    COMMAND |= 0x0240;
#else
    COMMAND |= 0x0040;  /* [10]-half duplex,[9]-MII mode,[6]-Pass runt frame,
    [5]-RxReset */
#endif
    /* back to back int-packet gap */
    IPGT = 0x0012;    /* IPG setting in half duplex mode */
  }
  else if ( (Speed == SPEED_10) && (Duplex == FULL_DUPLEX) )
  {
    MAC2 = 0x31;    /* full duplex, CRC and PAD enabled. */
    SUPP &= ~0x0100;  /* RMII Support Reg. speed is set to 10M */
#if RMII
    COMMAND |= 0x0640;
#else
    COMMAND |= 0x0440;  /* [10]-full duplex,[9]-MII mode,[6]-Pass runt frame,
    [5]-RxReset */
#endif
    /* back to back int-packet gap */
    IPGT = 0x0015;    /* IPG setting in full duplex mode */
  }
  else if ( (Speed == SPEED_100) && (Duplex == FULL_DUPLEX) )
  /* default setting, 100 BASE, FULL DUPLEX */
  {
    MAC2 = 0x31;    /* full duplex, CRC and PAD enabled. */
//    MAC2 = 0x01;
    SUPP |= 0x0100;   /* RMII Support Reg. speed is set to 100M */
#if RMII
    COMMAND |= 0x0640;
#else
    COMMAND |= 0x0440;  /* [10]-full duplex,[9]-MII mode,[6]-Pass runt frame,
    [5]-RxReset */
#endif
    /* back to back int-packet gap */
    IPGT = 0x0015;  /* IPG setting in full duplex mode */
  }

  EMACTxDescriptorInit();
  EMACRxDescriptorInit();

  MAC1 |= 0x0002;   /* [1]-Pass All Rx Frame */

  /* Set up RX filter, accept broadcast and perfect station */
#if ACCEPT_BROADCAST
  RXFILTERCTRL = 0x0022;  /* [1]-accept broadcast, [5]accept perfect */
#else
  RXFILTERCTRL = 0x0020;  /* accept perfect match only */
#endif

#if MULTICAST_UNICAST
  RXFILTERCTRL |= 0x0005;
#endif

#if ENABLE_HASH
  RXFILTERCTRL |= 0x0018;
#endif

  INTCLEAR = 0xFFFF;  /* clear all MAC interrupts */
  /* MAC interrupt related register setting */
  if ( install_irq( EMAC_INT, (void *)EMACHandler, HIGHEST_PRIORITY ) == FALSE )
  {
    return (FALSE);
  }

#if ENABLE_WOL
  RXFILTERWOLCLEAR = 0xFFFF;/* set all bits to clear receive filter WOLs */
  RXFILTERCTRL |= 0x2000; /* enable Rx Magic Packet and RxFilter Enable WOL */
  INTENABLE = 0x2000; /* only enable WOL interrupt */
#else
  INTENABLE = 0x00FF; /* Enable all interrupts except SOFTINT and WOL */
#endif
  return ( TRUE );
}

/*****************************************************************************
** Function name:   EMACReceiveFractions
**
** Descriptions:    Dealing with a fraction of EMAC packet
**
** parameters:      StartIndex and End Index
** Returned value:    packet length
**
*****************************************************************************/
DWORD EMACReceiveFractions( DWORD StartIndex, DWORD EndIndex )
{
DWORD i, RxLength = 0;
DWORD RxSize;
DWORD *rx_status_addr;

  for ( i = StartIndex; i < EndIndex; i++ )
  {
    /* Get RX status, two words, status info. and status hash CRC. */
    rx_status_addr = (DWORD *)(RX_STATUS_ADDR + StartIndex * 8);
    RxSize = (*rx_status_addr & DESC_SIZE_MASK) - 1;
    /* two words at a time, packet and control */
    CurrentRxPtr += EMAC_BLOCK_SIZE;
    StartIndex++;
    /* last fragment of a frame */
    if ( *rx_status_addr & RX_DESC_STATUS_LAST )
    {
      /* set INT bit and RX packet size */
      RXCONSUMEINDEX = StartIndex;
      RxLength += RxSize;
      break;
    }
    else  /* In the middle of the frame, the RxSize should be EMAC_BLOCK_SIZE */
          /* In the emac.h, the EMAC_BLOCK_SIZE has been set to the largest
          ethernet packet length to simplify the process, so, it should not
          come here in any case to deal with fragmentation. Otherwise,
          fragmentation and repacking will be needed. */
    {
      /* set INT bit and maximum block size */
      RXCONSUMEINDEX = StartIndex;
      /* wait until the whole block is received, size is EMAC_BLOCK_SIZE. */
      while ( (*rx_status_addr & DESC_SIZE_MASK) != (EMAC_BLOCK_SIZE - 1));
      RxLength += RxSize;
    }
  }
  return( RxLength );
}

/*****************************************************************************
** Function name:   EMACReceive
**
** Descriptions:    Receive a EMAC packet, called by ISR
**
** parameters:      buffer pointer
** Returned value:    packet length
**
*****************************************************************************/
DWORD EMACReceive( DWORD *EMACBuf )
{
DWORD RxProduceIndex, RxConsumeIndex;
DWORD RxLength = 0;
DWORD Counter = 0;

  /* the input parameter, EMCBuf, needs to be word aligned */

  RxProduceIndex = RXPRODUCEINDEX;
  RxConsumeIndex = RXCONSUMEINDEX;

  if ( RxProduceIndex == EMAC_RX_DESCRIPTOR_COUNT )
  {
    /* reach the limit, that probably should never happen */
    CurrentRxPtr = EMAC_RX_BUFFER_ADDR;
  }

  /* a packet has arrived. */
  if ( RxProduceIndex != RxConsumeIndex )
  {
    if ( RxProduceIndex < RxConsumeIndex )  /* Wrapped around already */
    {
      /* take care of unwrapped, RxConsumeIndex to EMAC_RX_DESCERIPTOR_COUNT */
      RxLength += EMACReceiveFractions( RxConsumeIndex, EMAC_RX_DESCRIPTOR_COUNT );
      Counter++;
      PacketReceived = TRUE;

      /* then take care of wrapped, 0 to RxProduceIndex */
      if ( RxProduceIndex > 0 )
      {
        RxLength += EMACReceiveFractions( 0, RxProduceIndex );
        Counter++;
      }
    }
    else          /* Normal process */
    {
      RxLength += EMACReceiveFractions( RxConsumeIndex, RxProduceIndex );
      Counter++;
    }
  }
  return( RxLength );
}

/*****************************************************************************
** Function name:   EMACSend
**
** Descriptions:    Send a EMAC packet
**
** parameters:      buffer pointer, buffer length
** Returned value:    true or false
**
*****************************************************************************/
DWORD EMACSend( DWORD *EMACBuf, DWORD length )
{
DWORD *tx_desc_addr;
DWORD TxProduceIndex;
DWORD TxConsumeIndex;
DWORD i, templen;

  TxProduceIndex = TXPRODUCEINDEX;
  TxConsumeIndex = TXCONSUMEINDEX;

  if ( TxConsumeIndex != TxProduceIndex )
  {
    return ( FALSE );
  }

  if ( TxProduceIndex == EMAC_TX_DESCRIPTOR_COUNT )
  {
    /* reach the limit, that probably should never happen */
    /* To be tested */
    TXPRODUCEINDEX = 0;
  }

  if ( length > EMAC_BLOCK_SIZE )
  {
    templen = length;
    for ( i = 0; (DWORD)(length/EMAC_BLOCK_SIZE) + 1; i++ )
    {
      templen = length - EMAC_BLOCK_SIZE;
      /* two words at a time, packet and control */
      tx_desc_addr = (DWORD *)(TX_DESCRIPTOR_ADDR + TxProduceIndex * 8);
      /* descriptor status needs to be checked first */
      if ( templen % EMAC_BLOCK_SIZE )
      {
        /* full block */
        *tx_desc_addr = (DWORD)(EMACBuf + i * EMAC_BLOCK_SIZE);
        /* set TX descriptor control field */
        *(tx_desc_addr+1) = (DWORD)(EMAC_TX_DESC_INT | (EMAC_BLOCK_SIZE - 1));
        TxProduceIndex++;
        if ( TxProduceIndex == EMAC_TX_DESCRIPTOR_COUNT )
        {
          TxProduceIndex = 0;
        }
        TXPRODUCEINDEX = TxProduceIndex;  /* transmit now */
      }
      else
      {
        /* last fragment */
        *tx_desc_addr = (DWORD)(EMACBuf + i * EMAC_BLOCK_SIZE);
        /* set TX descriptor control field */
        *(tx_desc_addr+1) = (DWORD)(EMAC_TX_DESC_INT | EMAC_TX_DESC_LAST | (templen -1) );
        TxProduceIndex++;   /* transmit now */
        if ( TxProduceIndex == EMAC_TX_DESCRIPTOR_COUNT )
        {
          TxProduceIndex = 0;
        }
        TXPRODUCEINDEX = TxProduceIndex;  /* transmit now */
        break;
      }
    }
  }
  else
  {
    tx_desc_addr = (DWORD *)(TX_DESCRIPTOR_ADDR + TxProduceIndex * 8);
    /* descriptor status needs to be checked first */
    *tx_desc_addr = (DWORD)(EMACBuf);
    /* set TX descriptor control field */
    *(tx_desc_addr+1) = (DWORD)(EMAC_TX_DESC_INT | EMAC_TX_DESC_LAST | (length -1));
    TxProduceIndex++;   /* transmit now */
    if ( TxProduceIndex == EMAC_TX_DESCRIPTOR_COUNT )
    {
      TxProduceIndex = 0;
    }
    TXPRODUCEINDEX = TxProduceIndex;
  }
  return ( TRUE );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
