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
 *    The IPC_Example shows how to use the IAR Embedded Workbench for ARM to
 *  develop code for Hitex-LPC4350 - evaluation board with LPC4350 dual-core
 *  processor. It shows how to exchange data between two cores using the IPC
 *  protocol. The application lights up the corresponding LED - LED0, LED1,
 *  LED2, LED3, when one or more sensor buttons are touched. USB0_IND LED
 *  toggles(blinks) with frequency which depends on the touched sensor button.
 *  If the button T1 is touched, the USB0_IND LED stops blinking.
 *
 *    The CortexM4 project is intended for the Cortex-M4 core and shows basic
 *  use of interrupt controller. It also provides basic initialization
 *  of the Cortex-M0 core, and brings it out of reset. The project uses IPC
 *  protocol to comunicate with the slave application(at cortex-m0 core). It
 *  sends various commands to the slave (for reading the keypad, switch-on or
 *  switch-off the LEDs or configuring the RI timer) and receives responses as
 *  IPC messages.
 *    If any FLASH configuration is selected, the linker compresses and embeds
 *  the CortexM0 application image to the program code. When the application
 *  is started, the image is decompressed to the internal RAM, prior to the
 *  Cortex-M0 reset release. This is done by a hidden code added by the compiler
 *  and executed before execution of main().
 *
 *      Note: If your application will be loaded to NORFLASH and is bigger than
 *            16Kb, verification may fail if you don't use the
 *            Hitex-LPC4350_NOR_debug.mac macro file when starting the
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
 *  see IPC_Example/Boot_Jumpers.png
 *
 *   Here is a diagram that shows how to configure the boot jumpers on the LPC4350 Hitex
 *  board. The boards are shipped with the center two positions shorted with jumpers.
 *
 *  See IPC_Example/Capture7.png
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
 *   The IPC_Example application is downloaded to NOR, SPI Flash (SPIFI), internal
 *  flash or RAM memory depending of the project's configuration and executed.
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
 *   2) Open second instance of IAR EWARM, open the IPC_Example and activate
 *      the CortexM0 project.
 *
 *   3) From the second instance choose the IRAM configuration, start the
 *      second debug session and run the CortexM0 application.
 *
 *    NORFLASH/SPIFI/IFLASH Configuration:
 *    --------------------------

 *   1) Activate the CortexM4 project and choose the NORFLASH/SPIFI/IFLASH
 *      configuration.
 *
 *   2) Open second instance of IAR EWARM, activate the CortexM0 project,
 *      choose NORFLASH/SPIFI/IFLASH configuration and build it. A binary image
 *      of the CortexM0 application will be created.
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
 *      Note: NORFLASH/SPIFI/IFLASH Batch build can be executed to build the
 *            NORFLASH/SPIFI/IFLASH configurations of both projects.
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
#include "drv_nvic.h"
#include "intrinsics.h"
#include "ipc.h"

#define IRC_FREQ       12000000
#define XTAL_FREQ      12000000

#define TASK_SYSTEM    0x0000
#define TASK_KEYPAD    0x0001
#define TASK_LEDS      0x0002
#define TASK_TIMER     0x0003

#define SERVICE_SYSTEM_INIT_END   0x00
#define SUBTASK_KEYPAD_READ       0x01
#define SUBTASK_LEDS_LATCH        0x01
#define SUBTASK_TIMER_STOP        0x01
#define SUBTASK_TIMER_CONFIGURE   0x02

/* Value for 0.25s timer interrupt @ 180MHz timer clock */
#define TIMER_VALUE_1     0x2AEA540

/* Value for 0.125s timer interrupt @ 180MHz timer clock */
#define TIMER_VALUE_2     0x15752A0

/* Value for 0.0625s timer interrupt @ 180MHz timer clock */
#define TIMER_VALUE_3     0x0ABA950

uint32_t ClockFrequency;

#define MULTF 15

#pragma section=".M0_CODE"

ipcValueMessage_t message;
ipcValueMessage_t * messagePtr;

volatile uint8_t flags[2] = {0,0};

#define flMessagePending        ( flags[0] )
#define flMessagePendingSet()   ( flags[0] = 0xFF )
#define flMessagePendingClear() ( flags[0] = 0x00 )

#define flUpdateLeds            ( flags[1] &   ( 1 << 0 ) )
#define flUpdateLedsSet()       ( flags[1] |=  ( 1 << 0 ) )
#define flUpdateLedsClear()     ( flags[1] &= ~( 1 << 0 ) )

#define flUpdateTimer           ( flags[1] &   ( 1 << 1 ) )
#define flUpdateTimerSet()      ( flags[1] |=  ( 1 << 1 ) )
#define flUpdateTimerClear()    ( flags[1] &= ~( 1 << 1 ) )

#define flStopTimer             ( flags[1] &   ( 1 << 2 ) )
#define flStopTimerSet()        ( flags[1] |=  ( 1 << 2 ) )
#define flStopTimerClear()      ( flags[1] &= ~( 1 << 2 ) )

#define flReadKeypad            ( flags[1] &   ( 1 << 3 ) )
#define flReadKeypadSet()       ( flags[1] |=  ( 1 << 3 ) )
#define flReadKeypadClear()     ( flags[1] &= ~( 1 << 3 ) )

#define flSlaveReady            ( flags[1] &   ( 1 << 4 ) )
#define flSlaveReadySet()       ( flags[1] |=  ( 1 << 4 ) )
#define flSlaveReadyClear()     ( flags[1] &= ~( 1 << 4 ) )

/* Symbols imported from the linker */
extern uint8_t HOST_CMD_BUFFER_start__;
extern uint8_t HOST_CMD_BUFFER_end__;
extern uint8_t HOST_MSG_BUFFER_start__;
extern uint8_t HOST_MSG_BUFFER_end__;

#define __cortex_m0_irq_enable()  NVIC_EnableIRQ(NVIC_M0CORE)
#define __cortex_m0_irq_disable() NVIC_DisableIRQ(NVIC_M0CORE)

volatile uint32_t errorCounterKeypad;
volatile uint32_t errorCounterLeds;
volatile uint32_t errorCounterTimer;

/*************************************************************************
 * Function Name: pushCommandToSlave
 * Parameters: task - task id,
 *             argument - argument id
 *             type - task type
 *             value - value to send
 *
 * Return: ipcQueueStatus_t: queue status
 *
 * Description: Creates and pushes command with the given parameters,
 *              to the command queue
 *
 *************************************************************************/
ipcQueueStatus_t pushCommandToSlave(uint16_t task, uint16_t argument, ipcCommands_t type, uint32_t value)
{
  ipcValueCommand_t command;
  ipcQueueStatus_t status;
  command.task = task;
  command.arg = argument;
  command.type = type;
  command.value = value;
  status = ipcMasterPushCommand((ipcValueCommand_t*)&command);
  if (status == QS_SUCCESS)
    ipcNotifySlave();
  return status;
}

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
}

/*************************************************************************
 * Function Name: CortexM0_IRQHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Handles interrupt from the Cortex M0 core
 *
 *************************************************************************/
void M0CORE_IRQHandler(void)
{
  flMessagePendingSet();
  /* Clear interrupt */
  CREG_M0TXEVENT_bit.TXEVCLR = 0;
  NVIC_ClearPendingIRQ(NVIC_M0CORE);
}

/*************************************************************************
 * Function Name: CortexM0_IRQ_Init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Initialize NVIC to handle interrupts from the
 *              CortexM0 core
 *
 *************************************************************************/
void M0CORE_IRQ_Init(void)
{
  CREG_M0TXEVENT_bit.TXEVCLR = 0;
  NVIC_ClearPendingIRQ(NVIC_M0CORE);
  NVIC_EnableIRQ(NVIC_M0CORE);
  NVIC_SetPriority(NVIC_M0CORE, (2 << 5));
}

uint32_t status_m,status_c;
uint32_t error=0;
uint32_t full=0;


/*************************************************************************
 * Function Name: handleKeypad
 * Parameters: state - current state of the handler
 *             message - pointer to the received message (if any)
 *
 * Return: uint32_t: new handler state
 *
 * Description: Handles the remote/slave keypad
 *
 *************************************************************************/

#define stKeypadIdle      0
#define stKeypadQuery     1
#define stKeypadWaiting   2
#define stKeypadResponded 3
#define stKeypadChanged   4
#define stKeypadError     5

uint32_t dataKeypad;
uint32_t dataKeypadOld;

uint32_t handleKeypad(uint32_t state, ipcValueMessage_t * message)
{
  switch(state)
  {
    case stKeypadIdle:
      if (flReadKeypad)
      {
        state = stKeypadQuery;
        flReadKeypadClear();
      }
      break;

    case stKeypadQuery:
      if(QS_SUCCESS != pushCommandToSlave(TASK_KEYPAD, SUBTASK_KEYPAD_READ, CMD_RD_ID, 0))
        state = stKeypadError;
      else
        state = stKeypadWaiting;
      break;

    case stKeypadWaiting:
      if(message != 0)
      if((TASK_KEYPAD == message->task) && (SUBTASK_KEYPAD_READ == message->arg))
      {
        if(MSG_RD_ID == message->type)
        {
          dataKeypad = message->value;
          state = stKeypadResponded;
        }
        else
          state = stKeypadError;
      }
      break;

    case stKeypadResponded:
      if (dataKeypad != dataKeypadOld)
      {
        dataKeypadOld = dataKeypad;
        state = stKeypadChanged;
      }
      else
        state = stKeypadIdle;
      break;

    case stKeypadChanged:
      if (flReadKeypad)
      {
        state = stKeypadQuery;
        flReadKeypadClear();
      }
      break;

    case stKeypadError:
      errorCounterKeypad++;
    default:
      state = stKeypadIdle;
      break;
  }
  return state;
}

/*************************************************************************
 * Function Name: handleLeds
 * Parameters: state - current state of the handler
 *             message - pointer to the received message (if any)
 *
 * Return: uint32_t: new handler state
 *
 * Description: Handles the remote/slave LEDs
 *
 *************************************************************************/
#define stLedsIdle       0
#define stLedsLatch      1
#define stLedsWaiting    2
#define stLedsLatched    3
#define stLedsError      4

volatile uint32_t dataLeds;

uint32_t handleLeds(uint32_t state, ipcValueMessage_t * message)
{
  switch(state)
  {
    case stLedsIdle:
      if (flUpdateLeds)
      {
        state = stLedsLatch;
        flUpdateLedsClear();
      }
      break;

    case stLedsLatch:
      if(QS_SUCCESS != pushCommandToSlave(TASK_LEDS, SUBTASK_LEDS_LATCH, CMD_WR_ID, dataLeds))
        state = stLedsError;
      else
        state = stLedsWaiting;
      break;

    case stLedsWaiting:
      if(message != 0)
      if((TASK_LEDS == message->task) && (SUBTASK_LEDS_LATCH == message->arg))
      {
        if(MSG_WR_STS_ID_SUCCESS == message->type)
          state = stLedsLatched;
        else
          state = stLedsError;
      }
      break;

    case stLedsLatched:
      state = stLedsIdle;
      break;

    case stLedsError:
      errorCounterLeds++;
    default:
      state = stLedsIdle;
      break;
  }
  return state;
}

/*************************************************************************
 * Function Name: handleTimer
 * Parameters: state - current state of the handler
 *             message - pointer to the received message (if any)
 *
 * Return: uint32_t: new handler state
 *
 * Description: Handles the remote/slave RI Timer
 *
 *************************************************************************/

#define stTimerStopped    0
#define stTimerStop       1
#define stTimerConfigure  2
#define stTimerConfigured 3
#define stTimerWaiting    4
#define stTimerError      5

uint32_t dataTimer;

uint32_t handleTimer(uint32_t state, ipcValueMessage_t * message)
{
  switch(state)
  {
    case stTimerStop:
      if(QS_SUCCESS != pushCommandToSlave(TASK_TIMER, SUBTASK_TIMER_STOP, CMD_WR_ID, 0))
        state = stTimerError;
      else
        state = stTimerWaiting;
      break;

    case stTimerStopped:
      if (flUpdateTimer)
      {
        state = stTimerConfigure;
        flUpdateTimerClear();
      }
      if (flStopTimer)
      {
        state = stTimerStop;
        flStopTimerClear();
      }
      break;

    case stTimerConfigure:
      if(QS_SUCCESS != pushCommandToSlave(TASK_TIMER, SUBTASK_TIMER_CONFIGURE, CMD_WR_ID, dataTimer))
        state = stTimerError;
      else
        state = stTimerWaiting;
      break;

    case stTimerWaiting:
      if(message != 0)
      if(TASK_TIMER== message->task)
      if(SUBTASK_TIMER_STOP == message->arg)
      {
        if(MSG_WR_STS_ID_SUCCESS == message->type)
          state = stTimerStopped;
        else
          state = stTimerError;
      }
      if(SUBTASK_TIMER_CONFIGURE == message->arg)
      {
        if(MSG_WR_STS_ID_SUCCESS == message->type)
          state = stTimerConfigured;
        else
          state = stTimerError;
      }
      break;

    case stTimerConfigured:
      if (flUpdateTimer)
      {
        state = stTimerConfigure;
        flUpdateTimerClear();
      }
      if (flStopTimer)
      {
        state = stTimerStop;
        flStopTimerClear();
      }
      break;

    case stTimerError:
      errorCounterTimer++;
    default:
      state = stTimerStop;
      break;
  }
  return state;
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
  uint32_t keypadState, ledsState, timerState;

  /* Configures Clock Generation Unit */
  InitClock();
  /* Force a RESET to Cortex-M0 */
  RGU_RESET_CTRL1 = ~RGU_RESET_ACTIVE_STATUS1 | RGU_RESET_CTRL1_M0APP_RST;
  /* Init interrupt from Cortex-M0 core */
  M0CORE_IRQ_Init();
  /* Init command and message queues used by IPC library */
  ipcMasterInitQueues((ipcCommandControlBlock_t*)&HOST_CMD_BUFFER_start__, \
                      (ipcCommand_t*)(&HOST_CMD_BUFFER_start__+sizeof(ipcCommandControlBlock_t)),\
                      (uint32_t)(&HOST_CMD_BUFFER_end__ - &HOST_CMD_BUFFER_start__ - sizeof(ipcCommandControlBlock_t)+1),\
                      (ipcMessageControlBlock_t*)&HOST_MSG_BUFFER_start__, \
                      (ipcMessage_t*)(&HOST_MSG_BUFFER_start__+sizeof(ipcMessageControlBlock_t)),\
                      (uint32_t)(&HOST_MSG_BUFFER_end__ - &HOST_MSG_BUFFER_start__ - sizeof(ipcMessageControlBlock_t)+1));
  /* Set start address(vector table) of Cortex-M0 application */
#if (__RAM_MODE__)
  CREG_M0APPMEMMAP = (uint32_t)0x10080000;
#else
  CREG_M0APPMEMMAP = (uint32_t)__section_begin(".M0_CODE");
#endif
  /* Release the Cortex-M0 core from RESET State */
  RGU_RESET_CTRL1 = ~RGU_RESET_ACTIVE_STATUS1 & ~RGU_RESET_CTRL1_M0APP_RST;
  /* Init flag, showing if the slave is ready */
  flSlaveReadyClear();

  while(1)
  {
    /* Gets a message from the message queue if any */
    messagePtr = 0;
    if (flMessagePending)
    {
      status_m = ipcMasterPopMessage(&message);
      /* Diabling interrupt while clearing the MessagePending flag,
         ensures that new message event cannot be missed            */
      __cortex_m0_irq_disable();
      if (!ipcQueueHasPendingMessages()) flMessagePendingClear();
      __cortex_m0_irq_enable();
      if (status_m != QS_SUCCESS) continue;
      messagePtr = &message;
    }
    /* If there is new message, and the message shows that slave is ready */
    if (messagePtr != 0)
    if ((messagePtr->task == TASK_SYSTEM) \
         && (messagePtr->arg == SERVICE_SYSTEM_INIT_END) \
         && (messagePtr->type == MSG_SRV_ID)) \
    {
      /* The slave is ready. Init handler states */
      flSlaveReadySet();
      keypadState = stKeypadIdle;
      ledsState = stLedsIdle;
      timerState = stTimerConfigured;
    }

    /* If the slave is not ready, skip handler functions */
    if (!flSlaveReady) continue;

    /* Handlers for various onboard modules */
    keypadState = handleKeypad(keypadState, messagePtr);
    ledsState = handleLeds(ledsState, messagePtr);
    timerState = handleTimer(timerState, messagePtr);

    /* If there is change in keypad state (button(s) touched or released) */
    if (stKeypadChanged == keypadState)
    {
      /* Set LEDs state, corresponding to the keypad state */
      dataLeds = dataKeypad;
      flUpdateLedsSet();

      /* Set RI Timer compare value, corresponding to the keypad state */
      switch (dataKeypad)
      {
        case 0x01:
          dataTimer = TIMER_VALUE_1;
          flUpdateTimerSet();
          break;

        case 0x02:
          dataTimer = TIMER_VALUE_2;
          flUpdateTimerSet();
          break;

        case 0x04:
          dataTimer = TIMER_VALUE_3;
          flUpdateTimerSet();
          break;

        case 0x08:
          flStopTimerSet();
          break;
      }
    }
    /* If keypad reading is complete, start new reading */
    if ((stKeypadChanged == keypadState) || (stKeypadIdle == keypadState))
     flReadKeypadSet();
  }
}
