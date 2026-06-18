/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : June 19, 2006
 *       Author      : Todor Atanasov
 *       Description : Created
 *
 *     2. Date        :July 18, 2006
 *       Author      : Todor Atanasov
 *       Description : - A/D conversion added (supporting the Status page where
 *                       the voltage regulated from the "AN_TR" is displayed).
 *                     - Button initialization added.
 *
 *    $Revision: 28 $
 *
 *
 *    DESCRIPTION
 *     ===========
 *   This example project is developed to show how to use the IAR Embedded
 *   Workbench for ARM and how to develop an application for the Olimex LPC-E2129
 *   evaluation boards. It shows a webserver application  running on the top of the
 *   uIP Emdedded TCP/IP Stack ver.1.0.
 *
 *   COMPATIBILITY
 *   =============
 *   The project is compatible with the Olimex LPC-E2129 evaluation board.
 *   The project is by default configured to use the J-Link JTAG interface.
 *
 *   GETTING STARTED
 *   ===============
 *   Start the IAR Embedded Workbench for ARM.
 *   Select File->Open->Workspace...
 *   Open the workspace file
 *   ...IAR Systems\Embedded Workbench 4.0\arm\src\examples\NXP\LPC212x
 *      \Olimex-P212X\eww\uip_webserver\uip_webserver.eww
 *
 *      - check the jumpers:
 *        DBG  - placed
 *        JRST - removed
 *      - power the LPC-E2129 board
 *      - compile, download and run program
 *
 *      The default IP address is:
 *      192.168.0.100
 *      The physical MAC address is (defined in cs8900a.h):
 *      00-ff-ff-ff-ff-ff
 *
 **************************************************************************
 * Copyright (c) 2001-2003, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the uIP TCP/IP stack.
 *
 **************************************************************************
 *
 *    $Revision: 28 $
 *
 **************************************************************************/
#include "includes.h"

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned int crp = 0xFFFFFFFF;

int AD_Result;
LPC_SysControl_MAMConfig_t MamConfig = {MAM_CONFIG,MAM_CYCLES};

/*************************************************************************
 * Function Name: irq_handler
 * Parameters:    none
 *
 * Return:        none
 *
 * Description:   IRQ handler
 *
 *************************************************************************/
__irq __arm void IRQ_Handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();   // Call vectored interrupt function.
  }
    VICVectAddr = 0;        // Clear interrupt in VIC.
}
/*************************************************************************
 * Function Name: uip_log
 * Parameters:    none
 *
 * Return:        none
 *
 * Description:   Events logging
 *
 *************************************************************************/
void uip_log (char *m)
{
  printf("uIP log message: %s\n", m);
}
/*************************************************************************
 * Function Name:   main
 * Parameters:      none
 *
 * Return:          none
 *
 * Description:     main
 *
 *************************************************************************/
int main(void)
{
unsigned int i;
uip_ipaddr_t ipaddr;
struct timer periodic_timer, arp_timer;

  printf("***** SYSTEM INITIALIZATION ***** ");

  SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, MAM_STATE, &MamConfig, 0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0);
  // Led initialization
  IO0DIR_bit.P0_8 = 1;     //set port0.8  as output
  IO0DIR_bit.P0_10 = 1;    //set port0.10 as output
  IO0DIR_bit.P0_11 = 1;    //set port0.11 as output

  IO0SET_bit.P0_8 = 1;     //LED1 OFF
  IO0SET_bit.P0_10 = 1;    //LED2 OFF
  IO0SET_bit.P0_11 = 1;    //LED3 OFF

  // Buttons initialization
  IO0DIR_bit.P0_15 = 0;    //set port0.15 as input
  IO0DIR_bit.P0_9 = 0;     //set port0.9  as input

  // Initialize VIC
  VIC_Init();
  VIC_SetProtectionMode(UserandPrivilegedMode);

  // Sys timer init 1/100 sec tick
  clock_init();

  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);

  // Initialize the ethernet device driver
  cs8900a_init();
  // Init A/D converter
  ADCR = 0x00201001;                    // SEL    = 0x01 - Channel 0 is selected
                                        // CLDDIV = 0x10
                                        // BURST  = 0    - software mode
                                        // CLKS   = 000  - 11clocks / 10bits
                                        // PDN    = 1    - A/D converter is operating
                                        // TEST1:0= 0    - normal operation
                                        // START  = 000
                                        // EDGE   = 0
  ADCR_bit.START = 1;                   // Start A/D convertion
  // Enable interrupts
  __enable_interrupt();

  // uIP web server
  // Initialize the uIP TCP/IP stack.
  uip_init();

  uip_ipaddr(ipaddr, 192,168,0,100);
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, 192,168,0,1);
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, 255,255,255,0);
  uip_setnetmask(ipaddr);

  // Initialize the HTTP server.
  httpd_init();

  while(1)
  {
    if(ADDR_bit.DONE)
    {
        AD_Result = ADDR_bit.RESULT;
        ADCR_bit.START = 1;             // Start A/D convertion
    }
    uip_len = cs8900a_poll();
    if(uip_len > 0)
    {
      if(BUF->type == htons(UIP_ETHTYPE_IP))
      {
	      uip_arp_ipin();
	      uip_input();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
	        uip_arp_out();
            cs8900a_send();
	      }
      }
      else if(BUF->type == htons(UIP_ETHTYPE_ARP))
      {
        uip_arp_arpin();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
            cs8900a_send();
	      }
      }
    }
    else if(timer_expired(&periodic_timer))
    {
      timer_reset(&periodic_timer);
      for(i = 0; i < UIP_CONNS; i++)
      {
      	uip_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0)
        {
          uip_arp_out();
          cs8900a_send();
        }
      }
#if UIP_UDP
      for(i = 0; i < UIP_UDP_CONNS; i++) {
        uip_udp_periodic(i);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0) {
          uip_arp_out();
          cs8900a_send();
        }
      }
#endif /* UIP_UDP */
      /* Call the ARP timer function every 10 seconds. */
      if(timer_expired(&arp_timer))
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
  }
}
