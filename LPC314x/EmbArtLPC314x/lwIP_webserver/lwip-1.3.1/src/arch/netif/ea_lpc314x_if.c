/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : ea_lpc314x_if.c
 *    Description : This file is dervied from the ``ethernetif.c'' skeleton
 *                  Ethernet network interface driver for lwIP.
 *
 *    History :
 *    1. Date        : 27.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *    2. Date        : 2.10.2009
 *       Author      : Stanimir Bonev
 *       Description : Adapt for an Embedded Artsists LPC314x board
 *
 *    $Revision: 28 $
 **************************************************************************/
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include <NXP/iolpc3141.h>
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "netif/etharp.h"
#include "netif/ppp_oe.h"
#include "netif/ea_lpc314x_if.h"
#include "arm_comm.h"
#include "drv_cgu.h"
#include "drv_intc.h"
#include "netif/dm9000_ea_lpc314x.h"

/**
 * Sanity Check:  This interface driver will NOT work if the following defines
 * are incorrect.
 *
 */
#if (PBUF_LINK_HLEN != 16)
#error "PBUF_LINK_HLEN must be 16 for this interface driver!"
#endif
#if (!SYS_LIGHTWEIGHT_PROT)
#error "SYS_LIGHTWEIGHT_PROT must be enabled for this interface driver!"
#endif

/**
 * Number of pbufs supported in low-level tx/rx pbuf queue.
 *
 */
#ifndef NUM_PBUF_QUEUE
#define NUM_PBUF_QUEUE    20
#endif

/**
 * Setup processing for PTP (IEEE-1588).
 *
 */
#if LWIP_PTPD
extern void lwIPHostGetTime(u32_t *time_s, u32_t *time_ns);
#endif

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 '0'

#define _MNAND_RYBN3 (1UL << 24)

/* Helper struct to hold a queue of pbufs for transmit and receive. */
struct pbufq {
  struct pbuf *pbuf[NUM_PBUF_QUEUE];
  unsigned long qwrite;
  unsigned long qread;
  unsigned long overflow;
};

/* Helper macros for accessing pbuf queues. */
#define PBUF_QUEUE_EMPTY(q) \
    (((q)->qwrite == (q)->qread) ? TRUE : FALSE)

#define PBUF_QUEUE_FULL(q) \
    ((((((q)->qwrite + 1) % NUM_PBUF_QUEUE)) == (q)->qread) ? \
    TRUE : FALSE )

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
  struct pbufq txq;
  struct pbufq rxq;
};

/**
 * Global variable for this interface's private data.  Needed to allow
 * the interrupt handlers access to this information outside of the
 * context of the lwIP netif.
 *
 */
static struct ethernetif ethernetif_data;


/**
 * Event router interrupt 0 handler
 *
 * @param q is the packet queue from which to pop the pbuf.
 *
 * @return pointer to pbuf packet if available, NULL otherswise.
 */
static  void evn_router_interrupt0_handler (void)
{
extern struct netif lwip_netif;
  ER_INT_CLR0 = _MNAND_RYBN3;
  ea_lpc314xif_interrupt(&lwip_netif);
  ea_lpc314xif_input(&lwip_netif);
}

/**
 * Pop a pbuf packet from a pbuf packet queue
 *
 * @param q is the packet queue from which to pop the pbuf.
 *
 * @return pointer to pbuf packet if available, NULL otherswise.
 */
static struct pbuf *
dequeue_packet(struct pbufq *q)
{
  struct pbuf *pBuf;
  SYS_ARCH_DECL_PROTECT(lev);

  /**
   * This entire function must run within a "critical section" to preserve
   * the integrity of the transmit pbuf queue.
   *
   */
  SYS_ARCH_PROTECT(lev);

  if(PBUF_QUEUE_EMPTY(q)) {
    /* Return a NULL pointer if the queue is empty. */
    pBuf = (struct pbuf *)NULL;
  }
  else {
    /**
     * The queue is not empty so return the next frame from it
     * and adjust the read pointer accordingly.
     *
     */
    pBuf = q->pbuf[q->qread];
    q->qread = ((q->qread + 1) % NUM_PBUF_QUEUE);
  }

  /* Return to prior interrupt state and return the pbuf pointer. */
  SYS_ARCH_UNPROTECT(lev);
  return(pBuf);
}

/**
 * Push a pbuf packet onto a pbuf packet queue
 *
 * @param p is the pbuf to push onto the packet queue.
 * @param q is the packet queue.
 *
 * @return 1 if successful, 0 if q is full.
 */
static int
enqueue_packet(struct pbuf *p, struct pbufq *q)
{
  SYS_ARCH_DECL_PROTECT(lev);
  int ret;

  /**
   * This entire function must run within a "critical section" to preserve
   * the integrity of the transmit pbuf queue.
   *
   */
  SYS_ARCH_PROTECT(lev);

  if(!PBUF_QUEUE_FULL(q)) {
    /**
     * The queue isn't full so we add the new frame at the current
     * write position and move the write pointer.
     *
     */
    q->pbuf[q->qwrite] = p;
    q->qwrite = ((q->qwrite + 1) % NUM_PBUF_QUEUE);
    ret = 1;
  }
  else {
    /**
     * The stack is full so we are throwing away this value.  Keep track
     * of the number of times this happens.
     *
     */
    q->overflow++;
    ret = 0;
  }

  /* Return to prior interrupt state and return the pbuf pointer. */
  SYS_ARCH_UNPROTECT(lev);
  return(ret);
}

/**
 * In this function, the hardware should be initialized.
 * Called from stellarisif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
__root Int32U ID;
static void
low_level_init(struct netif *netif)
{
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;
  netif->hwaddr[0] = 0x00;
  netif->hwaddr[1] = 0x12;
  netif->hwaddr[2] = 0x34;
  netif->hwaddr[3] = 0x56;
  netif->hwaddr[4] = 0x78;
  netif->hwaddr[5] = 0x9A;

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  // Init interface
  DM9000A_init_16bit();

  /* Do whatever else is needed to initialize interface. */
  /* Disable all Ethernet Interrupts. */
  CGU_Run_Clock(EVENT_ROUTER_PCLK);
  ER_MASK_CLR0 = _MNAND_RYBN3;


  /* Initialize the Ethernet Controller. */
  /*
   * Configure the Ethernet Controller for normal operation.
   * - Enable TX Duplex Mode
   * - Enable TX Padding
   * - Enable TX CRC Generation
   * - Enable RX Multicast Reception
   */

  DM9000A_reg_write(DM9000_GPR,0);
  DM9000A_reg_write(DM9000_NCR,NCR_RST);
  for(volatile Int32U i = 100; i;i--);
  DM9000A_reg_write(DM9000_NCR,0);

  // set MAC address
  for(Int32U i = 0;ETHARP_HWADDR_LEN > i; i++)
  {
    DM9000A_reg_write(DM9000_PAR+i,netif->hwaddr[i]);
  }

  DM9000A_reg_write(DM9000_TCSCSR,0);
  DM9000A_reg_write(DM9000_RCSCSR,0);

  /* Enable the Ethernet Controller transmitter and receiver. */
  DM9000A_reg_write(DM9000_RCR,RCR_RXEN);

  /* Enable the Ethernet Interrupt handler. */
  ER_INT_CLR0 = _MNAND_RYBN3;
  ER_APR0_bit.mNAND_RYBN3 = 1;  // active high
  ER_ATR0_bit.mNAND_RYBN3 = 0;  // direct
  ER_MASK_SET0 = _MNAND_RYBN3;
  ER_INTOUTMASKSET00 = _MNAND_RYBN3; // route to output 0
  /*Install Interrupt Service Routine*/
  INTC_IRQInstall(evn_router_interrupt0_handler, CASCADED_IRQ_0,
                  15,0);
  /*Enable Event router output 0 interrupt*/
  INTC_IntEnable(CASCADED_IRQ_0, 1);

  /* Enable Ethernet TX and RX Packet Interrupts. */
  DM9000A_reg_write(DM9000_IMR,IMR_PAR | IMR_PRM | IMR_PTM);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf might be
 * chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 * @note This function MUST be called with interrupts disabled or with the
 *       Stellaris Ethernet transmit fifo protected.
 */
static err_t
low_level_transmit(struct netif *netif, struct pbuf *p)
{
  unsigned char *pucBuf;
  struct pbuf *q;
  unsigned short Gather;
  unsigned short size;

  /* Initialize the gather register. */
  size = 0;

  iowr(DM9000A_CS_CMD_BASE_ADDR,DM9000_MWCMD);

  /* Copy data from the pbuf(s) into the TX Fifo. */
  for(q = p; q != NULL; q = q->next)
  {
    pucBuf = (unsigned char *)q->payload;
    for(Int32U i = 0; i < q->len; i++)
    {
      if(0 == (1 & size))
      {
        Gather = *pucBuf++;
      }
      else
      {
        Gather |= (*pucBuf++)<<8;
        iowr(DM9000A_CS_DAT_BASE_ADDR,Gather);
      }
      ++size;
    }
  }

  if (1 & size)
  {
    iowr(DM9000A_CS_DAT_BASE_ADDR,Gather);
  }

  // load data size
  DM9000A_reg_write(DM9000_TXPLH,size>>8);
  DM9000A_reg_write(DM9000_TXPLL,size);

  /* Wakeup the transmitter. */
  DM9000A_reg_write(DM9000_TCR,TCR_TXREQ);

  /* Dereference the pbuf from the queue. */
  pbuf_free(p);

  LINK_STATS_INC(link.xmit);

  return(ERR_OK);
}

/**
 * This function with either place the packet into the Stellaris transmit fifo,
 * or will place the packet in the interface PBUF Queue for subsequent
 * transmission when the transmitter becomes idle.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 */
static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct ethernetif *ethernetif = netif->state;
  unsigned short reg;

  SYS_ARCH_DECL_PROTECT(lev);

  /**
   * This entire function must run within a "critical section" to preserve
   * the integrity of the transmit pbuf queue.
   *
   */
  SYS_ARCH_PROTECT(lev);

  /**
   * Bump the reference count on the pbuf to prevent it from being
   * freed till we are done with it.
   *
   */
  pbuf_ref(p);

  /**
   * If the transmitter is idle, and there is nothing on the queue,
   * send the pbuf now.
   *
   */
  reg = DM9000A_reg_read(DM9000_TCR);
  if(PBUF_QUEUE_EMPTY(&ethernetif->txq) &&
    (0 == (TCR_TXREQ & reg)))
  {
    low_level_transmit(netif, p);
  }
  /* Otherwise place the pbuf on the transmit queue. */
  else {
    /* Add to transmit packet queue */
    if(!enqueue_packet(p, &(ethernetif->txq))) {
      /* if no room on the queue, free the pbuf reference and return error. */
      pbuf_free(p);
      SYS_ARCH_UNPROTECT(lev);
      return (ERR_MEM);
    }
  }

  /* Return to prior interrupt state and return. */
  SYS_ARCH_UNPROTECT(lev);
  return ERR_OK;
}

/**
 * This function will read a single packet from the Stellaris ethernet
 * interface, if available, and return a pointer to a pbuf.  The timestamp
 * of the packet will be placed into the pbuf structure.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return pointer to pbuf packet if available, NULL otherswise.
 */
static struct pbuf *
low_level_receive(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len;
  unsigned short *ptr;

  /* Check if a packet is available, if not, return NULL packet. */
  len = DM9000A_reg_read(DM9000_MRCMDX) & 0x3;


  if (1 == len)
  {
    // Get data
    DM9000A_reg_read(DM9000_MRCMD);
    // 16 bit
    len = iord(DM9000A_CS_DAT_BASE_ADDR);
  }
  else
  {
    return(NULL);
  }

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  /* If a pbuf was allocated, read the packet into the pbuf. */
  if(p != NULL)
  {
    /* Process all but the last buffer in the pbuf chain. */
    q = p;
    while(q != NULL)
    {
      /* Setup a byte pointer into the payload section of the pbuf. */
      ptr = q->payload;

      /**
       * Read data from FIFO into the current pbuf
       * (assume pbuf length is modulo 4)
       *
       */
      for(Int32U i = (q->len+1)>>1;i;--i)
      {

        *ptr++ = iord(DM9000A_CS_DAT_BASE_ADDR);
      }
      /* Link in the next pbuf in the chain. */
      q = q->next;
    }

    /* Adjust the link statistics */
    LINK_STATS_INC(link.recv);

  }
  /* If no pbuf available, just drain the RX fifo. */
  else
  {
    for(Int32U i = (len+1)>>1; i; --i)
    {
      iord(DM9000A_CS_DAT_BASE_ADDR);
    }
    /* Adjust the link statistics */
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }
  return(p);
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
int
ea_lpc314xif_input(struct netif *netif)
{
  struct ethernetif *ethernetif;
  struct pbuf *p;
  int count = 0;

  ethernetif = netif->state;

  /* move received packet into a new pbuf */
  while((p = dequeue_packet(&ethernetif->rxq)) != NULL) {
    count++;
    /* process the packet. */
    if (ethernet_input(p, netif)!=ERR_OK) {
      LWIP_DEBUGF(NETIF_DEBUG, ("stellarisif_input: input error\n"));
      pbuf_free(p);
      p = NULL;
    }
  }

  return(count);
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ea_lpc314xif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 1000000);

  netif->state = &ethernetif_data;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  ethernetif_data.ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  ethernetif_data.txq.qread = ethernetif_data.txq.qwrite = 0;
  ethernetif_data.txq.overflow = 0;
  ethernetif_data.rxq.qread = ethernetif_data.rxq.qwrite = 0;
  ethernetif_data.rxq.overflow = 0;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/**
 * Process tx and rx packets at the low-level interrupt.
 *
 * Should be called from the Stellaris Ethernet Interrupt Handler.  This
 * function will read packets from the Stellaris Ethernet fifo and place them
 * into a pbuf queue.  If the transmitter is idle and there is at least one packet
 * on the transmit queue, it will place it in the transmit fifo and start the
 * transmitter.
 *
 */
void
ea_lpc314xif_interrupt(struct netif *netif)
{
  struct ethernetif *ethernetif;
  struct pbuf *p = NULL;
  u16_t reg;

  /* setup pointer to the if state data */
  ethernetif = netif->state;

  /**
   * Process the transmit and receive queues as long as there is receive
   * data available
   *
   */
  reg = DM9000A_reg_read(DM9000_ISR);
  if(ISR_PRS & reg)
  {
    p = low_level_receive(netif);
    while(p)
    {
      DM9000A_reg_write(DM9000_ISR,ISR_PRS);
      /* Add the rx packet to the rx queue */
      if(!enqueue_packet(p, &ethernetif->rxq)) {
        /* Could not place the packet on the queue, bail out. */
        pbuf_free(p);
      }
      // send packet
      reg = DM9000A_reg_read(DM9000_NSR);
      if((NSR_TX2END | NSR_TX1END) & reg)
      {
        DM9000A_reg_write(DM9000_ISR,ISR_PTS);
        p = dequeue_packet(&ethernetif->txq);
        if(p != NULL) {
          low_level_transmit(netif, p);
        }
      }
      p = low_level_receive(netif);
    }
  }

  /* One more check of the transmit queue/fifo */
  reg = DM9000A_reg_read(DM9000_NSR);
  if((NSR_TX2END | NSR_TX1END) & reg)
  {
    DM9000A_reg_write(DM9000_ISR,ISR_PTS);
    p = dequeue_packet(&ethernetif->txq);
    if(p != NULL) {
      low_level_transmit(netif, p);
    }
  }
}

#if NETIF_DEBUG
/* Print an IP header by using LWIP_DEBUGF
 * @param p an IP packet, p->payload pointing to the IP header
 */
void
ea_lpc314xif_debug_print(struct pbuf *p)
{
  struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;
  u16_t *plen = (u16_t *)p->payload;

  LWIP_DEBUGF(NETIF_DEBUG, ("ETH header:\n"));
  LWIP_DEBUGF(NETIF_DEBUG, ("Packet Length:%5"U16_F" \n",*plen));
  LWIP_DEBUGF(NETIF_DEBUG, ("Destination: %02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"\n",
    ethhdr->dest.addr[0],
    ethhdr->dest.addr[1],
    ethhdr->dest.addr[2],
    ethhdr->dest.addr[3],
    ethhdr->dest.addr[4],
    ethhdr->dest.addr[5]));
  LWIP_DEBUGF(NETIF_DEBUG, ("Source: %02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"-%02"X8_F"\n",
    ethhdr->src.addr[0],
    ethhdr->src.addr[1],
    ethhdr->src.addr[2],
    ethhdr->src.addr[3],
    ethhdr->src.addr[4],
    ethhdr->src.addr[5]));
  LWIP_DEBUGF(NETIF_DEBUG, ("Packet Type:0x%04"U16_F" \n", ethhdr->type));
}
#endif /* NETIF_DEBUG */
