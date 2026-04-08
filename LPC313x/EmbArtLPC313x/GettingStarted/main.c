/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description :
 *
 *   This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for a Embedded Artsists LPC313x evaluation board. It shows
 *  basic use of I/O, Timers, ADC, Interrup controller, SPI and SDRAM. The projec
 *  also shows how the system can boot from the SPI NOR Flash. 
 *  It starts by blinking CPIO2_BOOT2 LED. The blinking period can be changed 
 *  by the trimer
 *   
 *COMPATIBILITY
 *=============
 *
 *   The Getting Started example project is compatible with Embedded Artsists
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
 *    1. Date        : 18.3.2009
 *       Author      : Stoyan Choynev
 *       Description : initial revision. 
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include <NXP/iolpc3131.h>
#include "arm926ej_cp15_drv.h"
#include "arm_comm.h"
#include "drv_cgu.h"
#include "drv_spi.h"
#include "drv_spinor.h"
#include "drv_intc.h"
#include "math.h"
/** local definitions **/
#define ISROM_MMU_TTBL  0x1201C000
#define SDRAM_BASE_ADDR 0x30000000
#define IRAM_BASE_ADDR  0x11028000

#define GPIO2_BOOT_SET  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_SET  = 4;}
#define GPIO2_BOOT_CLR  {IOCONF_GPIO_M1_SET = 4;IOCONF_GPIO_M0_CLR  = 4;}


#define   TIMER1_IN_FREQ       6MHZ
#define   TIMER1_TICK_MAX      1000   //in miliseconds
#define   TIMER1_TICK_MIN      20     //in miliseconds

#define   ADC_MAX              1024

#define   EXP_COEF             0.0038 //it is ln(TIMER1_TICK_MAX/TIMER1_TICK_MIN)/ADC_MAX

#define   TMR1TICK(tick)        ((TIMER1_IN_FREQ/1000)*tick)/256 //Convert miliseconds in Tmr ticks

/** external functions **/
extern void InitSDRAM(void);
extern void Dly_us(Int32U Dly);

/** external data **/
Int32U Tmr1Tick = TMR1TICK(100);
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
  
  if(IOCONF_GPIO_M0)
  {
    GPIO2_BOOT_CLR;
  }
  else
  {
    GPIO2_BOOT_SET;
  }
}

/*************************************************************************
 * Function Name: ADCIntrHandler
 * Parameters: None
 *
 * Return: None
 *
 * Description: ADC Interrupt service routine. Take Cannel 2 result.
 *              Calculate Tmr1Tick. Start new AD Conversion
 *
 *************************************************************************/
void ADCIntrHandler (void)
{
volatile Int32U Dummy = ADC_INT_STATUS_REG;
volatile Int32U Result;

    ADC_INT_CLEAR_REG  = 0x1;
     
    Result = ADC_R2_REG & 0x3FF;

   for (Int32U dummy_ctr = 0; dummy_ctr < 1000 ; dummy_ctr ++);
   /* Note JMI: I don't really understand this but that dummy
   counter seems to be required.
   If not implemented, the ISR is executed twice per conversion */
 
    if(ADC_INT_STATUS_REG)
    {
      ADC_INT_CLEAR_REG  = 0x1;
    }
    
    while(ADC_INT_STATUS_REG);
    
   /*Timer period is linear funcion of ADC value.*/
    //Int32U tmp = (Result*(TIMER1_TICK_MAX-TIMER1_TICK_MIN)/ADC_MAX+TIMER1_TICK_MIN);
   /*Timer period is quadrature funcion of ADC value.*/ 
    //Int32U tmp = (Result*Result*(TIMER1_TICK_MAX-TIMER1_TICK_MIN))/(ADC_MAX*ADC_MAX)+TIMER1_TICK_MIN;
   /*Timer period is exponential funcion of ADC value.*/ 
    Int32U tmp = TIMER1_TICK_MIN*exp(EXP_COEF*Result);

    Tmr1Tick = TMR1TICK(tmp);

    ADC_CON_REG_bit.START = 1;
   
    ADC_CON_REG_bit.START = 0;
}

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/
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
  Timer1Ctrl_bit.PreScale = 2;  // prescaler = CGU clock rate / 256
  Timer1Load = Tmr1Tick;
 // set timer 0 period
  Timer1Clear = 0;              // clear timer pending interrupt
  /*Install Interrupt Service Routine, 
    Priority 3*/
  INTC_IRQInstall(Timer1IntrHandler, TIMER0_INTCT1,
                  3,0);
  /*Enable Timer 1 interrupt*/
  INTC_IntEnable(TIMER0_INTCT1, 1);
  

  /*Enable ADC Clocks*/
  CGU_Run_Clock(ADC_PCLK);
  CGU_Run_Clock(ADC_CLK);
  /*ADC Reset*/
  ADC_PRESETN_SOFT = 1;
  ADC_RESETN_ADC10BITS_SOFT = 1;
  /*Stop ADC*/
  ADC_CON_REG = 0;
  /*ADC Init*/
  volatile Int32U Dummy = ADC_INT_STATUS_REG;
  ADC_INT_CLEAR_REG = 1;
  while(ADC_INT_STATUS_REG);
  ADC_INT_ENABLE_REG = 1;
  ADC_CON_REG_bit.SELREF = 1;
  /*10-bit resolution on channel 2*/
  ADC_CSEL_REG_bit.CSEL2 = 10;
  /*ADC Enable*/
  ADC_CON_REG_bit.ENABLE = 1;
  /*Install Interrupt Service Routine, 
    Lowest Priority*/
  INTC_IRQInstall(ADCIntrHandler, SSA1_ADC_INT,
                  1,0);

  /*Enable ADC interrupt*/
  INTC_IntEnable(SSA1_ADC_INT, 1);
  
  __enable_irq();
 
  /*Start Timer 1*/
  Timer1Ctrl_bit.Enable = 1;
  /*Start AD converstion*/
  ADC_CON_REG_bit.START = 1;
  ADC_CON_REG_bit.START = 0;

  GPIO2_BOOT_SET;
  
  while(1)
  {
    
  }
}


