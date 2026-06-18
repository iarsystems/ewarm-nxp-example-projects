/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description :
 *
 *  This example application demonstrates the operation of the LPC313X and
 * DM9000A Ethernet controller using the lwIP TCP/IP Stack. The project can be
 * configured to use Static IP or DHCP. The address that is selected will be shown
 * on the IO windows.
 *
 *COMPATIBILITY
 *=============
 *
 *   The WEB serever example project is compatible with Embedded Artsists
 *  LPC313x evaluation board. By default the project is configured to use the
 *  J-Link JTAG interface.
 *
 *CONFIGURATION
 *=============
 *
 *  The Project contains the following configurations:
 *
 *  Debug_iRAM: This configuration is intended for debugging in the
 *              microcontroller's internal RAM.
 *
 *  Debud_SDRAM: This configuration is intended for debugging in the external
 *               SDRAM.
 *
 *  Debug_SPINOR_Boot_iRAM: This confuguration will download the example in the
 *                          SPI NOR flash. The application is linked to run in
 *                          the internal RAM and is loaded by LPC IROM
 *                          bootloader. The configuration uses
 *                          FlashEmbArtLPC313x_boot.xml flash loader.
 *                          Note: This configuration uses a Hardware reset with
 *                          delay to start IROM bootloader and allow it to copy
 *                          the application. The boot jumpers should be
 *                          configured for SPI NOR flash boot.
 *
 * Debug_SPINOR_Boot_SDRAM: This confuguration will download the example in the
 *                          SPI NOR flash. The application is linked to run in
 *                          the external SDRAM and a second level bootloader is
 *                          linked to run in the internal RAM. The IROM
 *                          bootloader copies the secon level bootloader from
 *                          the SPI NOR to the interan RAM and starts it.
 *                          The second level bootloader copies the rest of the
 *                          application to SDRAM. The configuration uses
 *                          FlashEmbArtLPC313x_boot.xml flash loader to program
 *                          the secon level bootloader and
 *                          FlashEmbArtLPC313x_img.xml flash loader to program
 *                          the application.
 *
 *                          Note: This configuration uses a Hardware reset with
 *                          delay to start IROM bootloader and allow it to copy
 *                          the second level bootloader. The boot jumpers should
 *                          be configured for SPI NOR flash boot.
 *
 *  Jumpers:
 *   (Boot from SPI Flash)
 *    BOOT0      - L
 *    BOOT1      - L
 *    BOOT2      - H
 *
 *    History :
 *    1. Date        : 28.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include <assert.h>
#include <stdio.h>
#include <NXP/iolpc3131.h>

#include "arm926ej_cp15_drv.h"
#include "arm_comm.h"
#include "drv_cgu.h"
#include "drv_spi.h"
#include "drv_spinor.h"
#include "drv_intc.h"
#include "math.h"

#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/netifapi.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "lwip/mem.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "netif/etharp.h"


/** local definitions **/
#define ISROM_MMU_TTBL  0x1201C000
#define SDRAM_BASE_ADDR 0x30000000
#define IRAM_BASE_ADDR  0x11028000

#define ASSERT  assert

#define IPADDR_USE_STATIC       0
#define IPADDR_USE_DHCP         1
#define IPADDR_USE_AUTOIP       2

#define GPIO2_BOOT_SET  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_SET  = 4;}
#define GPIO2_BOOT_CLR  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_CLR  = 4;}

#define   TIMER1_IN_FREQ       6MHZ

#define   TMR1TICK(tick)        ((TIMER1_IN_FREQ/1000)*tick)/16 //Convert miliseconds in Tmr ticks

#define   LED_TIMER_MSECS       (100)

#define IPADDR(a,b,c,d)         ((a<<24) | (b<<16) | (c<<8) | d)
/** external functions **/
extern void InitSDRAM(void);
extern void Dly_us(Int32U Dly);
extern int  ea_lpc313xif_input(struct netif *netif);
extern err_t ea_lpc313xif_init(struct netif *netif);
extern void httpd_init(void);

/** external data **/
Int32U Tmr1Tick = TMR1TICK(1);

/** internal functions **/

/** public data **/
//*****************************************************************************
//
// The lwIP network interface structure for the Stellaris Ethernet MAC.
//
//*****************************************************************************
struct netif lwip_netif;

/** private data **/
//*****************************************************************************
//
// The local time for the lwIP Library Abstraction layer, used to support the
// Host and lwIP periodic callback functions.
//
//*****************************************************************************
static unsigned long g_ulLocalTimer = 0;

//*****************************************************************************
//
// The local time when the TCP timer was last serviced.
//
//*****************************************************************************
static unsigned long g_ulTCPTimer = 0;

//*****************************************************************************
//
// The local time when the HOST timer was last serviced.
//
//*****************************************************************************
#if HOST_TMR_INTERVAL
static unsigned long g_ulHostTimer = 0;
#endif

//*****************************************************************************
//
// The local time when the ARP timer was last serviced.
//
//*****************************************************************************
#if LWIP_ARP
static unsigned long g_ulARPTimer = 0;
#endif

//*****************************************************************************
//
// The local time when the AutoIP timer was last serviced.
//
//*****************************************************************************
#if LWIP_AUTOIP
static unsigned long g_ulAutoIPTimer = 0;
#endif

//*****************************************************************************
//
// The local time when the DHCP Coarse timer was last serviced.
//
//*****************************************************************************
#if LWIP_DHCP
static unsigned long g_ulDHCPCoarseTimer = 0;
#endif

//*****************************************************************************
//
// The local time when the DHCP Fine timer was last serviced.
//
//*****************************************************************************
#if LWIP_DHCP
static unsigned long g_ulDHCPFineTimer = 0;
#endif

//*****************************************************************************
//
// The default IP address aquisition mode.
//
//*****************************************************************************
static unsigned long g_ulIPMode = IPADDR_USE_STATIC;

//*****************************************************************************
//
// The local time when the LED timer was last serviced.
//
//*****************************************************************************
static unsigned long g_ulLedTimer = 0;

//*****************************************************************************
//
// The most recently assigned IP address.  This is used to detect when the IP
// address has changed (due to DHCP/AutoIP) so that the new address can be
// printed.
//
//*****************************************************************************
static unsigned long g_ulLastIPAddr = 0;

/** public functions **/

/** private functions **/
static void lwIPHostTimerHandler(void);


/*************************************************************************
 * Function Name: Timer1IntrHandler
 * Parameters: None
 *
 * Return: None
 *
 * Description: Timer 1 Interrupt service routine. Reload Timer
 *              with Tmr1Tick and toggle the GPIO2_BOOT LED
 *
 *************************************************************************/
void Timer1IntrHandler (void)
{
  Timer1Load = Tmr1Tick;
  Timer1Clear = 0;              // clear timer interrupt
  ++g_ulLocalTimer;

  //
  // Service the host timer.
  //
#if HOST_TMR_INTERVAL
  if((g_ulLocalTimer - g_ulHostTimer) >= HOST_TMR_INTERVAL)
  {
    g_ulHostTimer = g_ulLocalTimer;
    lwIPHostTimerHandler();
  }
#endif

  //
  // Service the ARP timer.
  //
#if LWIP_ARP
  if((g_ulLocalTimer - g_ulARPTimer) >= ARP_TMR_INTERVAL)
  {
    g_ulARPTimer = g_ulLocalTimer;
    etharp_tmr();
  }
#endif

  //
  // Service the TCP timer.
  //
  if((g_ulLocalTimer - g_ulTCPTimer) >= TCP_TMR_INTERVAL)
  {
    g_ulTCPTimer = g_ulLocalTimer;
    tcp_tmr();
  }

  //
  // Service the AutoIP timer.
  //
#if LWIP_AUTOIP
  if((g_ulLocalTimer - g_ulAutoIPTimer) >= AUTOIP_TMR_INTERVAL)
  {
    g_ulAutoIPTimer = g_ulLocalTimer;
    autoip_tmr();
  }
#endif

  //
  // Service the DCHP Coarse Timer.
  //
#if LWIP_DHCP
  if((g_ulLocalTimer - g_ulDHCPCoarseTimer) >= DHCP_COARSE_TIMER_MSECS)
  {
    g_ulDHCPCoarseTimer = g_ulLocalTimer;
    dhcp_coarse_tmr();
  }
#endif

  //
  // Service the DCHP Fine Timer.
  //
#if LWIP_DHCP
  if((g_ulLocalTimer - g_ulDHCPFineTimer) >= DHCP_FINE_TIMER_MSECS)
  {
    g_ulDHCPFineTimer = g_ulLocalTimer;
    dhcp_fine_tmr();
  }
#endif

  if((g_ulLocalTimer - g_ulLedTimer) >= LED_TIMER_MSECS)
  {
    g_ulLedTimer = g_ulLocalTimer;
    if(IOCONF_GPIO_M0)
    {
      GPIO2_BOOT_CLR;
    }
    else
    {
      GPIO2_BOOT_SET;
    }
  }
  ea_lpc313xif_input(&lwip_netif);
}

//*****************************************************************************
//
// Required by lwIP library to support any host-related timer functions.
//
//*****************************************************************************
#if HOST_TMR_INTERVAL
static
void lwIPHostTimerHandler(void)
{
    unsigned long ulIPAddress;

    //
    // Get the local IP address.
    //
    ulIPAddress = lwip_netif.ip_addr.addr;

    //
    // See if an IP address has been assigned.
    //
    //
    // Check if IP address has changed, and display if it has.
    //
    if(ulIPAddress != g_ulLastIPAddr)
    {
        //
        // Display the new IP address.
        //
        printf("\rIP: %d.%d.%d.%d       \n", ulIPAddress & 0xff,
               (ulIPAddress >> 8) & 0xff, (ulIPAddress >> 16) & 0xff,
               (ulIPAddress >> 24) & 0xff);

        //
        // Save the new IP address.
        //
        g_ulLastIPAddr = ulIPAddress;

        //
        // Display the new network mask.
        //
        ulIPAddress = lwip_netif.netmask.addr;
        printf("Netmask: %d.%d.%d.%d\n", ulIPAddress & 0xff,
               (ulIPAddress >> 8) & 0xff, (ulIPAddress >> 16) & 0xff,
               (ulIPAddress >> 24) & 0xff);

        //
        // Display the new gateway address.
        //
        ulIPAddress = lwip_netif.gw.addr;
        printf("Gateway: %d.%d.%d.%d\n", ulIPAddress & 0xff,
               (ulIPAddress >> 8) & 0xff, (ulIPAddress >> 16) & 0xff,
               (ulIPAddress >> 24) & 0xff);
    }
}
#endif

//*****************************************************************************
//
//! Initializes the lwIP TCP/IP stack.
//!
//! \param pucMAC is a pointer to a six byte array containing the MAC
//! address to be used for the interface.
//! \param ulIPAddr is the IP address to be used (static).
//! \param ulNetMask is the network mask to be used (static).
//! \param ulGWAddr is the Gateway address to be used (static).
//! \param ulIPMode is the IP Address Mode.  \b IPADDR_USE_STATIC will force
//! static IP addressing to be used, \b IPADDR_USE_DHCP will force DHCP with
//! fallback to Link Local (Auto IP), while \b IPADDR_USE_AUTOIP will force
//! Link Local only.
//!
//! This function performs initialization of the lwIP TCP/IP stack for the
//! Stellaris Ethernet MAC, including DHCP and/or AutoIP, as configured.
//!
//! \return None.
//
//*****************************************************************************
void
lwIPInit(const unsigned char *pucMAC, unsigned long ulIPAddr,
         unsigned long ulNetMask, unsigned long ulGWAddr,
         unsigned long ulIPMode)
{
    struct ip_addr ip_addr;
    struct ip_addr net_mask;
    struct ip_addr gw_addr;

    //
    // Check the parameters.
    //
#if LWIP_DHCP && LWIP_AUTOIP
    ASSERT((ulIPMode == IPADDR_USE_STATIC) ||
           (ulIPMode == IPADDR_USE_DHCP) ||
           (ulIPMode == IPADDR_USE_AUTOIP));
#elif LWIP_DHCP
    ASSERT((ulIPMode == IPADDR_USE_STATIC) ||
           (ulIPMode == IPADDR_USE_DHCP));
#elif LWIP_AUTOIP
    ASSERT((ulIPMode == IPADDR_USE_STATIC) ||
           (ulIPMode == IPADDR_USE_AUTOIP));
#else
    ASSERT(ulIPMode == IPADDR_USE_STATIC);
#endif

    //
    // Initialize lwIP library modules
    //
    lwip_init();

    //
    // Setup the network address values.
    //
    if(ulIPMode == IPADDR_USE_STATIC)
    {
        ip_addr.addr = htonl(ulIPAddr);
        net_mask.addr = htonl(ulNetMask);
        gw_addr.addr = htonl(ulGWAddr);
    }
#if LWIP_DHCP || LWIP_AUTOIP
    else
    {
        ip_addr.addr = 0;
        net_mask.addr = 0;
        gw_addr.addr = 0;
    }
#endif

    //
    // Create, configure and add the Ethernet controller interface with
    // default settings.
    //
    netif_add(&lwip_netif, &ip_addr, &net_mask, &gw_addr, NULL,
              ea_lpc313xif_init, ip_input);
    netif_set_default(&lwip_netif);

    //
    // Indicate that we are running in static IP mode.
    //
    g_ulIPMode = ulIPMode;

    //
    // Start DHCP, if enabled.
    //
#if LWIP_DHCP
    if(g_ulIPMode == IPADDR_USE_DHCP)
    {
        dhcp_start(&lwip_netif);
    }
#endif

    //
    // Start AutoIP, if enabled and DHCP is not.
    //
#if LWIP_AUTOIP
    if(g_ulIPMode == IPADDR_USE_AUTOIP)
    {
        autoip_start(&lwip_netif);
    }
#endif

    //
    // Bring the interface up.
    //
    netif_set_up(&lwip_netif);
}

/*************************************************************************
 * Function Name: main
 * Parameters: None
 *
 * Return: None
 *
 * Description: Getting Started main loop
 *
 *************************************************************************/
void main(void)
{
unsigned char pucMACArray[8];

  CP15_Mmu(FALSE);            // Disable MMU
  CP15_ICache(TRUE);          // Enable ICache

 // Init MMU
 // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_SetTtb((Int32U *)ISROM_MMU_TTBL);  //Set translation table base address
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

#ifndef DBG_SDRAM
  /*Init Clocks only if debug is not in SDRAM.
    if DBG_SDRAM the Clock is init by bootloader
    or mac file*/
  InitClock();
  /*Init SDRAM only if debug is not in SDRAM.
    if DBG_SDRAM the SDRAM is init by bootloader
    or mac file*/
  InitSDRAM();
#endif /*DBG_SDRAM*/

  /*Init SPI Nor Flash*/
  InitSPINOR();
  /*Init Interrupt Controller.
    Arm Vector Copy to beginnint of the IRAM*/
  INTC_Init((Int32U *)IRAM_BASE_ADDR);
  /*Remap IRAM at address 0*/
  SYSCREG_ARM926_901616_LP_SHADOW_POINT = IRAM_BASE_ADDR;
  /*Enable Timer 1 Clock*/
  CGU_Run_Clock(TIMER1_PCLK);
  /*Init Timer 1*/
  Timer1Ctrl_bit.Enable = 0;    // disable counting
  Timer1Ctrl_bit.Mode = 1;      // reload mode
  Timer1Ctrl_bit.PreScale = 1;  // prescaler = CGU clock rate / 16
  Timer1Load = Tmr1Tick;
  // set timer 0 period
  Timer1Clear = 0;              // clear timer pending interrupt
  /*Install Interrupt Service Routine,
    Priority 3*/
  INTC_IRQInstall(Timer1IntrHandler, TIMER0_INTCT1,
                  3,0);
  /*Enable Timer 1 interrupt*/
  INTC_IntEnable(TIMER0_INTCT1, 1);

  /*Start Timer 1*/
  Timer1Ctrl_bit.Enable = 1;
  GPIO2_BOOT_SET;

  
#if LWIP_DHCP
  //
  // Initialze the lwIP library, using DHCP.
  //
  lwIPInit(pucMACArray, 0, 0, 0, IPADDR_USE_DHCP);
#else
  //
  // Initialze the lwIP library, using Static IP.
  //
  lwIPInit(pucMACArray, IPADDR(192,168,0,100), IPADDR(255,255,255,0), \
                        IPADDR(192,168,0,1), IPADDR_USE_STATIC);  
#endif
  
  __enable_irq();

  //
  // Initialize a sample httpd server.
  //
  httpd_init();

  while(1)
  {

  }
}


