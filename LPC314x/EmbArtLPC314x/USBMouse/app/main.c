/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description :
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for a Embedded Artsists LPC314x evaluation board. It implements
 *  USB HID mouse.
 *   The joystick control up, down, left, right and left mouse button.
 *  The projec also shows how the system can boot from the SPI NOR Flash.
 *
 *COMPATIBILITY
 *=============
 *
 *   The USB Mouse example project is compatible with Embedded Artsists
 *  LPC314x evaluation board. By default the project is configured to use the
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
 *  Jumpers:
 *   (Boot from SPI Flash)
 *    BOOT0 - L
 *    BOOT1 - L
 *    BOOT2 - H
 *
 *    History :
 *    1. Date        : 7.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *    2. Date        : 2.10.2009
 *       Author      : Stanimir Bonev
 *       Description : Adapt for an Embedded Artsists LPC314x board
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include "includes.h"

/** local definitions **/
#define ISROM_MMU_TTBL  0x1201C000
#define SDRAM_BASE_ADDR 0x30000000
#define IRAM_BASE_ADDR  0x11028000

#define GPIO2_BOOT_SET  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_SET  = 4;}
#define GPIO2_BOOT_CLR  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_CLR  = 4;}


#define   TIMER1_IN_FREQ       6MHZ
#define   TIMER1_TICK_MAX      1000   //in miliseconds
#define   TIMER1_TICK_MIN      20     //in miliseconds

#define   TMR1TICK(tick)       ((TIMER1_IN_FREQ/1000)*tick)/256 //Convert miliseconds in Tmr ticks

#define   MOUSE_DELTA          1

/** external functions **/
extern void InitSDRAM(void);
extern void Dly_us(Int32U Dly);

/** external data **/
Int32U Tmr1Tick = TMR1TICK(2);

volatile Boolean Tick;

/** internal functions **/
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

  Tick = TRUE;
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
Int32U blink_led_div = 0;
Boolean led_state = FALSE;
Buttons_t ButtCtrl;
Int8S X = 0, Y = 0;
Int8U Buttons = 0, ButtonsHold = 0;

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
  SYSCREG_ARM926_SHADOW_POINTER = IRAM_BASE_ADDR;
  /*Enable Timer 1 Clock*/
  CGU_Run_Clock(TIMER1_PCLK);
  /*Init Timer 1*/
  Timer1Ctrl_bit.Enable = 0;    // disable counting
  Timer1Ctrl_bit.Mode = 1;      // reload mode
  Timer1Ctrl_bit.PreScale = 2;  // prescaler = CGU clock rate / 256
  Timer1Load = Tmr1Tick;
 // set timer 0 period
  Timer1Clear = 0;              // clear timer pending interrupt
  /*Install Interrupt Service Routine,
    Priority 3*/
  INTC_IRQInstall(Timer1IntrHandler, TIMER1_INTCT,
                  3,0);
  /*Enable Timer 1 interrupt*/
  INTC_IntEnable(TIMER1_INTCT, 1);

  // Init USB device Class
  HidInit();

  __enable_irq();

  // Buttons Init
  ButtonsInit();

  /* Start Timer 1*/
  Timer1Ctrl_bit.Enable = 1;

  // Soft connection enable
  USB_ConnectRes(TRUE);

  while(1)
  {
    if(Tick)
    {
      Tick = FALSE;
      if(100 < ++blink_led_div)
      {
        blink_led_div = 0;
        if(led_state ^= TRUE)
        {
          // LED ON
          GPIO2_BOOT_CLR;
        }
        else
        {
          GPIO2_BOOT_SET;
        }
      }
      ButtCtrl = GetButtons();
      // Buttons and Joystick processing
      if(ButtCtrl.JsUp)
      {
        Y -= MOUSE_DELTA;
      }
      else if(ButtCtrl.JsDown)
      {
        Y += MOUSE_DELTA;
      }

      if(ButtCtrl.JsRight)
      {
        X += MOUSE_DELTA;
      }
      else if(ButtCtrl.JsLeft)
      {
        X -= MOUSE_DELTA;
      }

      if(ButtCtrl.JsCenter || ButtCtrl.B1)
      {
        Buttons |= 1;
      }
      else
      {
        Buttons &= ~1;
      }

      if(ButtCtrl.B2)
      {
        Buttons |= 2;
      }
      else
      {
        Buttons &=~2;
      }

      if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
      {
        UsbWakeUp();  // wake-up
        // Send report
        if(   UsbDevStatusConfigured == UsbCoreReq(UsbCoreReqDevState)
           && !UsbCoreReq(UsbCoreReqDevSusState)
          )
        {
          HidMouseSendReport(X,Y,Buttons);
        }
        ButtonsHold = Buttons;
        Y = X = Buttons = 0;
      }
    }
  }
}


