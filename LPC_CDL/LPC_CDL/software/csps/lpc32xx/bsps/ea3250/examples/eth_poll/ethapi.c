/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2009 Embedded Artists AB
 *
 * Description:
 *    Sample application that implements ICMP (Ping) functionality.
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "ethapi.h"
#include <string.h>

/******************************************************************************
 * Defines
 *****************************************************************************/

#define ARP_REQUEST    1         /* Operation ARP request      */
#define ARP_REPLY      2         /* Operation ARP reply        */

/******************************************************************************
 * Local variables
 *****************************************************************************/

/*
 * The IP address assigned to the local host. 
 */ 
extern UNS_8 localIP[4];

/*
 * The Ethernet address (MAC) assigned to the local host
 */
extern UNS_8 mac[6];


UNS_8 inBuf[INBUF_LEN];

/*****************************************************************************
 *
 * Description:
 *    Checksum calculation. 16-bit one's complement. 
 *
 * Params:
 *    [in] pStartAddress - pointer to beginning of data 
 *    [in] len           - length of data 
 *    [in] initialValue  - precalculated checksum (e.g. pseudo header) 
 *
 * Returns:
 *    calculated checksum 
 *
 ****************************************************************************/
static UNS_16
checksum(UNS_8* pStartAddress,
         UNS_16 len,
         UNS_16 initialValue)
{
  UNS_32 csum       = ~initialValue & 0x0000ffff;
  UNS_8 *p8bit      = NULL;
  UNS_16 val16      = 0;

  UNS_32 aWordCount = len / 2;
  p8bit           = pStartAddress;

  while (aWordCount-- > 0)
  {
    val16  = *p8bit++;
    val16 |= *p8bit << 8;
    p8bit++;
    
    csum  += val16;
  }


  if (len % 2)
  {
    /* The segment ends with odd byte, add it. */
    csum += *p8bit++;
  }

  while (csum >> 16)
  {
    csum = (csum & 0xffff) + (csum >> 16); 
    /* add in end-around carry */
  }

  return ~(UNS_16) csum;
}

/*****************************************************************************
 *
 * Description:
 *    Handles an incoming ICMP packet
 *
 ****************************************************************************/
static void
icmpInput(UNS_8* pBuf,
          UNS_16 len)
{

  /*
   *     +--+--+--+--+
   *  0: |ty|co|chsum|
   *     +--+--+--+--+
   * 
   * ty  = type
   * co  = code
   * 
   */

  UNS_16 csum = 0;

  if(len < 4)
  {
    return;
  }


  if(checksum((UNS_8 *)pBuf, len, 0) != 0) 
  {
    return;
  }

  if(!(pBuf[0] == 8 && pBuf[1] == 0)) 
  {
    /* Only support for Ping */
    return;
  }

  // create reply

  pBuf[0]     = 0;

  pBuf[2] = 0;
  pBuf[3] = 0;

  csum = checksum(pBuf, len, 0);

  pBuf[2] = (csum & 0xff);
  pBuf[3] = (csum >> 8);

  // create IP header

  pBuf -= 20;
  len  += 20;

  // set destination IP address (same as source in input packet)
  memcpy(&pBuf[16], &pBuf[12], 4);

  // set source IP address
  memcpy(&pBuf[12], localIP, 4);

  // checksum
  pBuf[10] = 0;
  pBuf[11] = 0;

  csum = checksum(pBuf, 20, 0);

  pBuf[10] = (csum & 0xff);
  pBuf[11] = (csum >> 8);

  // ethernet header

  pBuf -= 14;
  len  += 14;

  // destination ethernet address
  memcpy(&pBuf[0], &pBuf[6], 6);

  memcpy(&pBuf[6], mac, 6);

  ethIf_send(pBuf, len);
}

/*****************************************************************************
 *
 * Description:
 *   Handles an incoming IP packet
 *
 ****************************************************************************/
static void
ipInput(UNS_8* pBuf, 
        UNS_16 len)

{
  UNS_16 totLen = 0;

  /*
   *     +--+--+--+--+--+--+--+--+
   *  0: |vl|TO| len |ident|flags|
   *     +--+--+--+--+--+--+--+--+
   *  8: |TT|pr|chsum|  src IP   |
   *     +--+--+--+--+--+--+--+--+
   * 16: |  dst IP   |
   *     +--+--+--+--+
   * 
   * vl  = version and header length
   * TO  = TOS
   * len = total length
   * TT  = TTL
   * pr  = protocol
   * 
   */

  if(len < 20)
  {
    return;
  }

  // version 4, header length 20 (5 << 2)
  if(pBuf[0] != 0x45)
  {
    return;
  }


  if (checksum(pBuf, 20, 0) != 0)
  {
    return;
  }

  // only accept packets destined directly to local host
  if(memcmp(&pBuf[16], localIP, 4) != 0)
  {
    return;
  }

  totLen = ((pBuf[2] << 8) | pBuf[3]);

  // only accept ICMP
  if(pBuf[9] == 1)
    icmpInput(pBuf+20, totLen-20);

}



/*****************************************************************************
 *
 * Description:
 *    Handles an incoming ARP packet. 
 *
 ****************************************************************************/
static void
arpInput(UNS_8* pBuf,
         UNS_16 len)
{
  BOOL_32 isLocalTarg = FALSE;
  UNS_16  op          = 0;

  /*
   *     +--+--+--+--+--+--+--+--+
   *  0: |  ht |  pt |hs|ps| op  |
   *     +--+--+--+--+--+--+--+--+
   *  8: |  sender eth     | send
   *     +--+--+--+--+--+--+--+--+
   * 16: | IP  |  target eth     |
   *     +--+--+--+--+--+--+--+--+
   * 24: | target IP |
   *     +--+--+--+--+
   *
   *
   * ht = hardware address type
   * pt = protocol address type
   * hs = hardware address size
   * ps = protocol address size
   * op = operation
   * 
   */

  if(len < 28)
    return;

  op = (pBuf[6] << 8 | pBuf[7]);
  isLocalTarg = memcmp(&pBuf[24], localIP, 4) == 0;
  
  if(op == ARP_REQUEST && isLocalTarg == TRUE)
  {
    // create a reply

    // set operation to ARP reply
    pBuf[6] = ((ARP_REPLY >> 8) & 0xff);
    pBuf[7] = (ARP_REPLY & 0xff);

    // copy sender ethernet address to target ethernet address position
    memcpy(&pBuf[18], &pBuf[8], 6);

    // copy sender IP address to target IP address position
    memcpy(&pBuf[24], &pBuf[14], 4);

    // copy local mac address to sender ethernet address position
    memcpy(&pBuf[8], mac, 6);

    // copy local IP address to sender IP address position
    memcpy(&pBuf[14], localIP, 4);


    // create ethernet header

    pBuf -= 14;

    // destination ethernet address
    memcpy(&pBuf[0], &pBuf[14+18], 6);

    // local ethernet address
    memcpy(&pBuf[6], mac, 6);

    // ethernet type (= ARP, 0x0806)
    pBuf[12] = (0x0806 >> 8);
    pBuf[13] = (0x0806 & 0xff);

    ethIf_send(pBuf, 42);
  }
}

/*****************************************************************************
 *
 * Description:
 *    Handles an incoming Ethernet packet. 
 *
 ****************************************************************************/
void
ethInput(UNS_8* pBuf, 
         UNS_16 len)
{
  /*
   * Ethernet header:
   * 
   * | dest addr | src addr | type |
   *       6          6         2      
   */

  UNS_16 type = 0;

  if(len < 14)
    return;

  type = (pBuf[12] << 8 | pBuf[13]);

  /* move data pointer past ethernet header */
  pBuf += 14;
  len  -= 14;

  if(type < 0x0800)
  {
    /* 802.2/802.3 not supported */

    return;
  }

  /* send the packet to the destination protocol */

  switch(type)
  {
  case 0x0800: /* IP packet */
    ipInput(pBuf, len);
    break;
  case 0x0806: /* ARP packet */
    arpInput(pBuf, len);
    break;
  default:
    break;
  }
}


