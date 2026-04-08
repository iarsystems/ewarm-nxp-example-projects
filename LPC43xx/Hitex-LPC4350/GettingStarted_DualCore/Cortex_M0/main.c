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
 *    The CortexM0 project is intended for the Cortex-M0 core and shows basic
 *  use of RI Timer, I/O and interrupt controller. The application waits for
 *  interrupts from the Cortex-M4 core. When there is an interrupt, the
 *  application reads from the shared memory which sensor button is touched.
 *  Then it updates RI timer's COMPVAL register with value, corresponding to
 *  the touched button, and the USB0_IND LED starts to blink with different
 *  frequency.
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
 *    IRAM - debugs the projects from the internal RAM.
 *
 *    NORFLASH - loads the project to external NOR flash.
 *               Only "Debug without Downloading" should be selected for the
 *               CortexM0 project.
 *
 *    SPIFI - loads project to external SPI flash. Only "Debug without 
 *            Downloading" should be selected for the CortexM0 project.
 *
 *    IFLASH - loads project to the internal flash (only if LPC43xx device with
 *             internal flash is present). Only "Debug without 
 *             Downloading" should be selected for the CortexM0 project.
 *
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
 *      choose NORFLASH/SPIFI/IFLASH configuration and build it. A binary
 *      image of the CortexM0 application will be created.
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
#include "modules\drv_nvic.h"

#pragma section=".shared_data"

#pragma pack(1)

typedef struct
{
  uint32_t Button;
  uint8_t  Flag;
} SharedData_t;

#pragma pack()

volatile SharedData_t SharedData @ ".shared_data";

/* Value for 1s timer interrupt @ 180MHz timer clock */
#define RIT_VALUE_INIT  0xABA9500

/* Value for 0.5s  timer interrupt @ 180MHz timer clock */
#define RIT_VALUE_1     0x55D4A80

/* Value for 0.25s timer interrupt @ 180MHz timer clock */
#define RIT_VALUE_2     0x2AEA540

/* Value for 0.125s timer interrupt @ 180MHz timer clock */
#define RIT_VALUE_3     0x15752A0

/* Value for 0.0625s timer interrupt @ 180MHz timer clock */
#define RIT_VALUE_4     0x0ABA950

volatile uint8_t flag0=0;

#define flTick               ( flag0 )
#define flTickSet()          ( flag0 = 0xFF )
#define flTickClear()        ( flag0 = 0x00 )

/*************************************************************************
 * Function Name: RIT_WWDT_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Repetitive interrupt timer & WWDT handler
 *
 *************************************************************************/
void M0_RITIMER_OR_WWDT_IRQHandler(void)
{
  if (RIT_CTRL_bit.RITINT)
  {
    flTickSet();
    /* Clear interrupt */
    RIT_CTRL_bit.RITINT = 1;
    NVIC_ClearPendingIRQ(NVIC_M0_RITIMER_OR_WWDT);
  }
}

/*************************************************************************
 * Function Name: RIT_Init
 * Parameters: compare value for RI Timer
 *
 * Return: none
 *
 * Description: Initialization of repetitive interrupt timer
 *
 *************************************************************************/
void RITIMER_Init(uint32_t value)
{
  /* Value for 0.25s timer interrupt @ 180MHz timer clock */
  RIT_COMPVAL = value;
  RIT_COUNTER = 0;
  RIT_CTRL_bit.RITEN = 1;
  RIT_CTRL_bit.RITENCLR = 1;
}

/*************************************************************************
 * Function Name: RIT_IRQ_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Configure NVIC for RI timer interrupt handling
 *
 *************************************************************************/
void RITIMER_IRQ_Init(void)
{
  /* Enable interrupt */
  NVIC_EnableIRQ(NVIC_M0_RITIMER_OR_WWDT);
  NVIC_SetPriority(NVIC_M0_RITIMER_OR_WWDT, (1 << 6));
}

/*************************************************************************
 * Function Name: CortexM4_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Handles interrupts from the Cortex-M4 core
 *              Initialize RIT with value, received from Cortex-M4
 *              application
 *
 *************************************************************************/
void M0_M4CORE_IRQHandler(void)
{
  switch(SharedData.Button)
  {
    case 0 : break;
    case 1 : RITIMER_Init(RIT_VALUE_1);
             break;
    case 2 : RITIMER_Init(RIT_VALUE_2);
             break;
    case 4 : RITIMER_Init(RIT_VALUE_3);
             break;
    case 8 : RITIMER_Init(RIT_VALUE_4);
             break;
    default: RITIMER_Init(RIT_VALUE_INIT);
             break;
  }
  /* Mark data as "read" */
  SharedData.Flag = 0;
  /* Clear interrupt */
  CREG_M4TXEVENT_bit.TXEVCLR = 0;
  NVIC_ClearPendingIRQ(NVIC_M0_M4CORE);
}

/*************************************************************************
 * Function Name: CortexM4_IRQ_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize NVIC to handle interrupts from the
 *              CortexM4 core
 *
 *************************************************************************/
void CortexM4_IRQ_Init(void)
{
  NVIC_EnableIRQ(NVIC_M0_M4CORE);
  NVIC_SetPriority(NVIC_M0_M4CORE, (2 << 6));
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
  /* Initialize Repetitve Interrupt Timer */
  RITIMER_Init(RIT_VALUE_INIT);
  /* Configure NVIC for RI Timer interrupt handling */
  RITIMER_IRQ_Init();
  /* Configure NVIC for Cortex-M4 interrupt handling */
  CortexM4_IRQ_Init();
  /* Configure the onboard LED - GPIO4.1 */
  GPIO_CLR4 = 1 << 1;
  GPIO_DIR4 |= 1 << 1;
  /* Clear shared data flag */
  SharedData.Flag = 0;
  while(1)
  {
    if (flTick)
    {
      flTickClear();
      /* Invert onboard LED */
      GPIO_NOT4 = 1 << 1;
    }
  }
}
