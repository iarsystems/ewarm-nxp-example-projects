/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    DESCRIPTION
 *    ===========
 *      This example project shows how to use the IAR Embedded Workbench for ARM
 *     to develop code for Hitex-LPC4350 evaluation board. It shows basic use of
 *     I/O, RI timer, I2C and interrupt controller.
 *     It toggles USB0_IND LED every 0.25s. Also lights up corresponding LED -
 *     LED0, LED1, LED2, LED3, when one or more sensor buttons are touched.
 *
 *      Note: If your application will be loaded to NORFLASH and is bigger than
 *            16Kb, verification may fail if you don't use the
 *            FlashHitexLPC4350_NOR_debug.mac macro file when starting the
 *            debugger. The macro file configures all of the address lines of EMC,
 *            in order to address the whole NORFLASH memory.
 *
 *    COMPATIBILITY
 *    =============
 *      The example project is compatible with Hitex LPC4350-A2 evaluation board.
 *
 *
 *    CONFIGURATION
 *    =============
 *
 *    Once you have successfully programmed the flash on the Hitex board, there is one
 *  step left before the LPC4350 can boot from external flash. That final step is to
 *  configure the boot mode using the boot mode jumpers.
 *
 *  see GettingStarted/Boot_Jumpers.png
 *
 *   Here is a diagram that shows how to configure the boot jumpers on the LPC4350 Hitex
 *  board. The boards are shipped with the center two positions shorted with jumpers.
 *
 *  See GettingStarted/Capture7.png
 *
 *  Jumper JP5 should be open.
 *
 *  Note: Boot config pin P2_9 must be pulled down to GND via 10k resistor
 *         to boot from external flash. On some early revision boards this pin
 *         cannot be configured by jumper and is default high level, so booting
 *         is not possible.
 *
 *  The GettingStarted application is downloaded to NOR, SPI Flash (SPIFI) or
 *  RAM memory depending of the project's configuration and executed.
 *
 *  IRAM - runs project from internal RAM.
 *
 *  NORFLASH - loads project to external NOR flash.
 *
 *  SPIFI - loads project to external SPI flash.
 *
 *  IFLASH - loads project to internal flash (only if LPC43xx device with internal
 *           flash is present).
 *
 *    GETTING STARTED
 *    ===============
 *
 *      1) Build and download the program.
 *
 *      2) Run the program.
 *
 *
 *    History :
 *    1. Date        : October 2011
 *       Author      : Atanas Uzunov
 *       Description : create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "NXP\iolpc4350.h"
#include "stdint.h"
#include "modules\i2c_drv.h"
#include "modules\drv_nvic.h"

#define IRC_FREQ       12000000
#define XTAL_FREQ      12000000

//nt32_t ClockFrequency;

uint32_t I2C0_PCLK;

unsigned char Data[2];

#define MULTF 15

#define PCA9502_ADDR  0x4D

volatile uint8_t flag0=0;

#define flTick               ( flag0 )
#define flTickSet()          ( flag0 = 0xFF )
#define flTickClear()        ( flag0 = 0x00 )

/*************************************************************************
 * Function Name: InitClock
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize PLL and clock dividers. FCCO = 320MHz,
 *              PLL1_OUT = 160MHz
 *
 *************************************************************************/
void InitClock(void)
{
  /* 1. Init XTAL OSC */
  CGU_XTAL_OSC_CTRL_bit.HF = 0;       /* Xtal.freq is between 1MHz and 15MHZ */
  CGU_XTAL_OSC_CTRL_bit.BYPASS = 0;   /* Xtal.osc is connected */
  CGU_XTAL_OSC_CTRL_bit.ENABLE = 0;   /* Xtal.osc is enabled */

  /* 2.  Wait for OSC ready > 2ms */
  for(volatile uint32_t j = 20000; j; j--);

  /* 3. Switch to OSC clk */
  /* BASE_M4_CLK */
  CGU_BASE_M4_CLK = (1ul  << 11)      /* Autoblock during freq.change enabled */
                  | (6ul  << 24)      /* Source is XTAL osc. */
                    ;
  /* BASE_APB1_CLK */
  CGU_BASE_APB1_CLK = (1ul  << 11)    /* Autoblock during freq.change enabled */
                    | (6ul  << 24)    /* Source is XTAL osc. */
                    ;
  /* BASE_APB3_CLK */
  CGU_BASE_APB3_CLK = (1ul  << 11)    /* Autoblock during freq.change enabled */
                    | (6ul  << 24)    /* Source is XTAL osc. */
                    ;
  /* BASE_SPIFI_CLK */
  CGU_BASE_SPIFI_CLK = (1ul  << 11)   /* Autoblock during freq.change enabled */
                     | (6ul  << 24)   /* Source is XTAL osc. */
                    ;

  /* 4. Init PLL1 - In clk - OSC (12MHz)
     N = 3, M = 80, P = 1 VCO - 320MHz, PLL1 OUT - 160MHz */
  CGU_PLL1_CTRL_bit.PD = 1;           /* Power down PLL1 */
  CGU_PLL1_CTRL_bit.BYPASS = 0;       /* CCO clock sent to post-dividers */
  CGU_PLL1_CTRL_bit.DIRECT = 0;       /* Direct CCO output disabled */
  CGU_PLL1_CTRL_bit.FBSEL = 0;        /* CCO output as feedback divider */
  CGU_PLL1_CTRL_bit.PSEL = 1-1;       /* Post divider P = 1 */
  CGU_PLL1_CTRL_bit.NSEL = 3-1;       /* N = 3 */
  CGU_PLL1_CTRL_bit.MSEL = 80-1;      /* M = 80 */
  CGU_PLL1_CTRL_bit.CLK_SEL = 6;      /* Source is XTAL osc. */
  CGU_PLL1_CTRL_bit.AUTOBLOCK = 1;    /* Autoblock during freq.change enabled */

  /* 5. Enable PLL1 */
  CGU_PLL1_CTRL_bit.PD = 0;           /* Power up (enable) PLL1 */

  /* 5. Wait for the PLL1 to achieve lock */
  while(!CGU_PLL1_STAT_bit.LOCK);

  /* 6. Dividers */
  /* IDIVA - /4 input 160MHz, output 40MHz */
  CGU_IDIVA_CTRL_bit.AUTOBLOCK = 1;   /* Autoblock during freq.change enabled */
  CGU_IDIVA_CTRL_bit.IDIV = 4-1;      /* IDIV = 4 */
  CGU_IDIVA_CTRL_bit.CLK_SEL = 9;     /* Source is PLL1 */
  CGU_IDIVA_CTRL_bit.PD = 0;          /* Enable IDIVA */

  /* 7. Init Clock output stages */
  /* BASE_M4_CLK, APB0, APB2 - 160MHz */
  CGU_BASE_M4_CLK = (1ul  << 11)      /* Autoblock during freq.change enabled */
                  | (9ul  << 24)      /* Source is PLL1 */
                    ;
  /* BASE_APB1_CLK - 160MHz*/
  CGU_BASE_APB1_CLK = (1ul  << 11)    /* Autoblock during freq.change enabled */
                    | (9ul << 24)     /* Source is PLL1 */
                    ;
  /* BASE_SPIFI_CLK - 40MHz */
  CGU_BASE_SPIFI_CLK = (1ul  << 11)   /* Autoblock during freq.change enabled */
                     | (12ul << 24)   /* Source is IDIVA */
                    ;
  /* Enable M4 Bus clock */
  CCU1_CLK_M4_BUS_CFG_bit.RUN = 1;

  /* Enable APB 1 Bus clock */
  CCU1_CLK_APB1_BUS_CFG_bit.RUN = 1;

  /* Enable SCU clock */
  CCU1_CLK_M4_SCU_CFG_bit.RUN = 1;

  /* Set I2C0 module frequency = APB1 freq */
  I2C0_PCLK = 160000000;
}

/*************************************************************************
 * Function Name: RIT_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Repetitive interrupt timer handler
 *
 *************************************************************************/
void RITIMER_IRQHandler(void)
{
    flTickSet();
    /* Clear interrupt */
    RIT_CTRL_bit.RITINT = 1;
    NVIC_ClearPendingIRQ(NVIC_RITIMER);
}

/*************************************************************************
 * Function Name: RIT_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialization of repetitive interrupt timer
 *
 *************************************************************************/
void RITIMER_Init(void)
{
  /* Value for 0.25s timer interrupt @ 180MHz timer clock */
  RIT_COUNTER = 0;
  RIT_COMPVAL = 0x2AEA540;
  RIT_CTRL_bit.RITEN = 1;
  RIT_CTRL_bit.RITENCLR = 1;
  /* Enable interrupt */
  NVIC_EnableIRQ(NVIC_RITIMER);
  NVIC_SetPriority(NVIC_RITIMER, 16);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main()
{
  /* Configures Clock Generation Unit */
  InitClock();
  /* Force a RESET to Cortex-M0 */
  RGU_RESET_CTRL1 = ~RGU_RESET_ACTIVE_STATUS1 | RGU_RESET_CTRL1_M0APP_RST;
  /* Configures Repetitve Interrupt Timer */
  RITIMER_Init();
  /* Configure the onboard LED - GPIO4.1 */
  GPIO_CLR4 = 1 << 1;
  GPIO_DIR4 |= 1 << 1;
  /* Init I2C */
  I2C_InitMaster(400000);
  /* Reset PCA9502 */
  Data[0] = 0x0E<<3;
  Data[1] = 0x01<<3;
  I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
  /* Configure LED pins as outputs */
  Data[0] = 0x0A<<3;
  Data[1] = 0x0F;
  I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
  while(1)
  {
    if (flTick)
    {
      flTickClear();
      /* Invert onboard LED */
      GPIO_NOT4 = 1 << 1;
    }
    /* Read state of sensor/touch-buttons */
    Data[0] = 0x0B<<3;
    I2C_MasterWrite(PCA9502_ADDR, &Data[0], 1);
    I2C_MasterRead(PCA9502_ADDR, &Data[1], 1);
    /* Write to PCA9502's output register - drive LEDs */
    Data[1] >>= 4;
    Data[1] = ~Data[1];
    I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
  }
}
