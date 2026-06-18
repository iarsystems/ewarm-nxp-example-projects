/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *
 *    DESCRIPTION
 *    ===========
 *    The GettingStarted_DualCore example shows how to use the IAR Embedded
 *  Workbench for ARM to develop code for Hitex-LPC4350 - evaluation board
 *  with LPC4350 dual-core processor. It shows how to excange data between
 *  2 cores. The application lights up corresponding LED - LED0, LED1, LED2,
 *  LED3, when one or more sensor buttons are touched. USB0_IND LED toggles
 *  with frequency which depends on the touched sensor button.
 *
 *    The CortexM4 project is intended for the Cortex-M4 core and shows basic
 *  use of I2C and interrupt controller.It also provides basic initialization
 *  of the Cortex-M0 core, and brings it out of reset. When one of the sensor
 *  buttons is touched, event is generated and the button number is sent to
 *  the Cortex-M0 core through a shared memory.
 *    If the NORFLASH/SPIFI configuration is selected, the linker compresses and
 *  embeds the CortexM0 application image to the program code. When the
 *  application is started, the image is decompressed to the internal RAM,
 *  prior to the Cortex-M0 reset release. This is done by a hidden code added
 *  by the compiler and executed before execution of main().
 *    If the IFLASH configuration is selected, the CortexM0 application is also
 *  embedded in the program code by the linker, but is run from the internal flash.
 *
 *      Note: If your application will be loaded to NORFLASH and is bigger than
 *            16Kb, verification may fail if you don't use the
 *            FlashHitexLPC4350_NOR_debug.mac macro file when starting the
 *            debugger. The macro file configures all of the address lines of
 *            EMC, in order to address the whole NORFLASH memory.
 *
 *
 *    COMPATIBILITY
 *    =============
 *    The example project is compatible with Hitex-LPC4350-A2 evaluation board.
 *
 *
 *    CONFIGURATION
 *    =============
 *
 *    Once you have successfully programmed the flash on the Hitex board, there is one
 *  step left before the LPC4350 can boot from external flash. That final step is to
 *  configure the boot mode using the boot mode jumpers.
 *
 *  see GettingStarted_DualCore/Boot_Jumpers.png
 *
 *   Here is a diagram that shows how to configure the boot jumpers on the LPC4350 Hitex
 *  board. The boards are shipped with the center two positions shorted with jumpers.
 *
 *  See GettingStarted_DualCore/Capture7.png
 *
 *   We can ship boards with two example projects flashed in them. The first loads when the
 *  board is configured to boot from Parallel Flash, and the second loads when the board
 *  is configured to boot from QSPI flash.
 *
 *  Jumper JP5 should be open.
 *
 *  Note: Boot config pin P2_9 must be pulled down to GND via 10k resistor
 *         to boot from external flash. On some early revision boards this pin
 *         cannot be configured by jumper and is default high level, so booting
 *         is not possible.
 *
 *  The GettingStarted_DualCore application is downloaded to NOR, SPI Flash (SPIFI) or
 *  RAM memory depending of the project's configuration and executed.
 *
 *  IRAM - runs project from internal RAM.
 *
 *  NORFLASH - loads project to external NOR flash.
 *
 *  SPIFI - loads project to external SPI flash.
 *
 *  IFLASH - loads project to the internal flash (only if LPC43xx device with
 *           internal flash is present).
 *
 *    GETTING STARTED
 *    ===============
 *
 *    IRAM Configuration:
 *    --------------------
 *
 *   1) Activate the CortexM4 project and choose the IRAM configuration.
 *      Start debug session and run the CortexM4 application.
 *
 *   2) Open second instance of IAR EWARM, open the GettingStarted_DualCore
 *      example and activate the CortexM0 project.
 *
 *   3) From the second instance choose the IRAM configuration, start the
 *      second debug session and run the CortexM0 application.
 *
 *    NORFLASH/SPIFI/IFLASH Configuration:
 *    ------------------------
 *
 *   1) Activate the CortexM4 project and choose the NORFLASH/SPIFI/IFLASH
 *      configuration.
 *
 *   2) Open second instance of IAR EWARM, activate the CortexM0 project,
 *      choose NORFLASH/SPIFI/IFLASH configuration and build it. A binary image
 *      of the CortexM0 application will be created.
 *
 *
 *   3) From the firstly opened instance of IAR EWARM with the activated
 *      CortexM4 project, start debug session and run the application.
 *      This action loads both CortexM4 and cortexM0 images to NORFLASH/SPIFI/
 *      IFLASH.
 *
 *   4) From the second instance of IAR EWARM, start debug session without
 *      downloading (Debug without Downloading) and run the CortexM0
 *      application.
 *
 *   Note: NORFLASH/SPIFI/IFLASH Batch build can be executed to build the
 *         NORFLASH/SPIFI/IFLASH configurations of both projects
 *
 *
 *    History :
 *    1. Date        : December 2011
 *       Author      : Atanas Uzunov
 *       Description : create
 *
 *    $Revision: 28 $
 **************************************************************************/

#include "NXP\iolpc4350.h"
#include "stdint.h"
#include "modules\i2c_drv.h"
#include "modules\drv_nvic.h"
#include "intrinsics.h"

#define IRC_FREQ       12000000
#define XTAL_FREQ      12000000

uint32_t ClockFrequency;

uint32_t I2C0_PCLK;

uint8_t Data[2];
uint8_t PrevButtonState;

#define MULTF 15

#define PCA9502_ADDR  0x4D

#pragma section=".M0_CODE"
#pragma section=".shared_data"

#pragma pack(1)

typedef struct
{
  uint32_t Button;
  uint8_t  Flag;
} SharedData_t;

#pragma pack()

volatile SharedData_t SharedData @ ".shared_data";

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

  /* Enable M0 Core clock */
  CCU1_CLK_M4_M0APP_CFG_bit.RUN = 1;

  /* Enable APB 1 Bus clock */
  CCU1_CLK_APB1_BUS_CFG_bit.RUN = 1;

  /* Enable SCU clock */
  CCU1_CLK_M4_SCU_CFG_bit.RUN = 1;

  /* Set I2C0 module frequency = APB1 freq */
  I2C0_PCLK = 160000000;
}

/*************************************************************************
 * Function Name: MessageCortexM0
 * Parameters: none
 *
 * Return: none
 *
 * Description: Sends button data to Cortex-M0 Applcation
 *
 *************************************************************************/
void MessageM0CORE(uint32_t data){
  /* If previous data is read by Cortex-M0 application */
  if (!SharedData.Flag)
  {
    /* Copy button data to shared data memory */
    SharedData.Button = data;
    /* Mark shared data as "not read" */
    SharedData.Flag = 1;
    /* Send Event to Cortex-M0 core*/
    __SEV();
  }
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

  /* Initialize shared data */
  SharedData.Button = 0;
  SharedData.Flag   = 0;

  /* Set start address(vector table) of Cortex-M0 application */
#if (__RAM_MODE__)
  CREG_M0APPMEMMAP = (uint32_t)0x10080000;
#else
  CREG_M0APPMEMMAP = (uint32_t)__section_begin(".M0_CODE");
#endif

  /* Release the Cortex-M0 core from RESET State */
  RGU_RESET_CTRL1 = ~RGU_RESET_ACTIVE_STATUS1 & ~RGU_RESET_CTRL1_M0APP_RST;

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
  /* Clears Previous Data variable */
  PrevButtonState = 0;

  while(1)
  {
    /* Read state of sensor/touch-buttons */
    Data[0] = 0x0B<<3;
    I2C_MasterWrite(PCA9502_ADDR, &Data[0], 1);
    I2C_MasterRead(PCA9502_ADDR, &Data[1], 1);
    Data[1] >>= 4;

    /* If there is a change in the state of buttons, signal CortexM0 */
    if (PrevButtonState != Data[1]) MessageM0CORE(Data[1]);
    /* Store the state of buttons */
    PrevButtonState = Data[1];

    /* Write to PCA9502's output register - drive LEDs */
    Data[1] = ~Data[1];
    I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
  }
}
