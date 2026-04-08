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
 *    The CortexM0 project is intended for the Cortex-M0 core and shows basic
 *  use of RI Timer, I2C, I/O and interrupt controller. The application waits
 *  for IPC commands from the master application(cortex-m4 core), executes them
 *  and sends back IPC response messages to the master. The IPC framework uses
 *  command and message buffers located into shared memory.
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
 *    IRAM     - debugs the projects from the internal RAM.
 *
 *    NORFLASH - loads the project to external NOR flash. Only "Debug without 
 *               Downloading" should be selected for the CortexM0 project.
 *
 *    SPIFI - loads project to external SPI flash. Only "Debug without 
 *            Downloading" should be selected for the CortexM0 project.
 *
 *    IFLASH - loads project to the internal flash (only if LPC43xx device with
 *             internal flash is present). Only "Debug without 
 *             Downloading" should be selected for the CortexM0 project.
 *
 *     Jumpers:
 *
 *     JP5 - open
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
 *   2) Open second instance of IAR EWARM, open the IPC_Example and activate
 *      the CortexM0 project.
 *
 *   3) From the second instance choose the IRAM configuration, start the
 *      second debug session and run the CortexM0 application.
 *
 *    NORFLASH Configuration:
 *    ------------------------
 *
 *   1) Activate the CortexM4 project and choose the NORFLASH configuration.
 *
 *   2) Open second instance of IAR EWARM/IPC_Example, activate the CortexM0
 *      project, choose NORFLASH configuration and build it. A binary image
 *      of the CortexM0 application will be created.
 *
 *   3) From the firstly opened instance of IAR EWARM with the activated
 *      CortexM4 project, start debug session and run the application.
 *      This action loads both CortexM4 and cortexM0 images to NORFLASH.
 *
 *   4) From the second instance of IAR EWARM, start debug session without
 *      downloading (Debug without Downloading) and run the CortexM0
 *      application.
 *
 *   Note: NORFLASH Batch build can be executed to build the NORFLASH
 *         configurations of both projects
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
#include "i2c_drv.h"
#include "ipc.h"
#include "intrinsics.h"

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

#define PCA9502_ADDR  0x4D

uint32_t I2C0_PCLK;

ipcValueCommand_t command;
ipcQueueStatus_t status_c;

uint32_t errorCounter;

uint8_t Data[2];

/* Value for 0.5s  timer interrupt @ 180MHz timer clock */
#define TIMER_VALUE_1     0x55D4A80

volatile uint8_t flags[2] = {0,0};

#define flTick                  ( flags[0] )
#define flTickSet()             ( flags[0] = 0xFF )
#define flTickClear()           ( flags[0] = 0x00 )

#define flCommandPending        ( flags[1] )
#define flCommandPendingSet()   ( flags[1] = 0xFF )
#define flCommandPendingClear() ( flags[1] = 0x00 )

/* Symbols imported from the linker */
extern uint8_t HOST_CMD_BUFFER_start__;
extern uint8_t HOST_CMD_BUFFER_end__;
extern uint8_t HOST_MSG_BUFFER_start__;
extern uint8_t HOST_MSG_BUFFER_end__;

#define __cortex_m4_irq_enable()  NVIC_EnableIRQ(NVIC_M0_M4CORE)
#define __cortex_m4_irq_disable() NVIC_DisableIRQ(NVIC_M0_M4CORE)

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
  RIT_COUNTER = 0;
  RIT_COMPVAL = value;
  RIT_CTRL_bit.RITINT = 1;
  RIT_CTRL_bit.RITEN = 1;
  RIT_CTRL_bit.RITENCLR = 1;
}

/*************************************************************************
 * Function Name: RIT_Stop
 * Parameters: none
 *
 * Return: none
 *
 * Description: Stops the RI Timer
 *
 *************************************************************************/
void RITIMER_Stop(void)
{
  /* Disable timer */
  RIT_CTRL_bit.RITEN = 0;
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
void M0_RITIMER_IRQInit(void)
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
  flCommandPendingSet();
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
void M0_M4CORE_IRQInit(void)
{
  CREG_M4TXEVENT_bit.TXEVCLR = 0;
  NVIC_ClearPendingIRQ(NVIC_M0_M4CORE);
  NVIC_EnableIRQ(NVIC_M0_M4CORE);
  NVIC_SetPriority(NVIC_M0_M4CORE, (2 << 6));
}

/*************************************************************************
 * Function Name: pushMessageToMaster
 * Parameters: task - task id,
 *             argument - argumend id,
 *             type - message type,
 *             value - value to send
 *
 * Return: status of the message queue
 *
 * Description: Pushes message with the given parameters to the
 *              message queue.
 *
 *************************************************************************/
ipcQueueStatus_t pushMessageToMaster(uint16_t task, uint16_t argument, ipcMessages_t type, uint32_t value)
{
  ipcValueMessage_t message;
  ipcQueueStatus_t status;
  message.task = task;
  message.arg = argument;
  message.type = type;
  message.value = value;
  status = ipcSlavePushMessage((ipcValueMessage_t*)&message);
  if (status == QS_SUCCESS)
    ipcNotifyMaster();
  return status;
}

/*************************************************************************
 * Function Name: readKeypad
 * Parameters: none
 *
 * Return: uint8_t: value of the PCA9502 input register
 *
 * Description: Loads value to the PCA9502 output register. Switch
 *              on the LED if the corresponding bit is 0, and switch
 *              off if the LED if bit is 1.
 *
 *************************************************************************/
uint8_t readKeypad()
{
  /* Read state of sensor/touch-buttons */
  Data[0] = 0x0B<<3;
  I2C_MasterWrite(PCA9502_ADDR, &Data[0], 1);
  I2C_MasterRead(PCA9502_ADDR, &Data[1], 1);
  Data[1] >>= 4;
  return Data[1];
}

/*************************************************************************
 * Function Name: handleKeypadCommand
 * Parameters: command - pointer to the command
 *
 * Return: uint32_t: 0 - command executed successfully
 *                   1 - error occured
 *
 * Description: Executes keypad related command.
 *
 *************************************************************************/
uint32_t handleKeypadCommand(ipcValueCommand_t * command)
{
  uint32_t data;
  if (TASK_KEYPAD != command->task) return 1;
  switch(command->arg)
  {
    /* If keypad should be read */
    case SUBTASK_KEYPAD_READ:
      if (command->type != CMD_RD_ID) return 1;
      data = (uint32_t)readKeypad();
      if (QS_SUCCESS != pushMessageToMaster(TASK_KEYPAD, SUBTASK_KEYPAD_READ, MSG_RD_ID, data)) return 1;
      break;
    /* Unknown subtask */
    default:
      if (QS_SUCCESS != pushMessageToMaster(TASK_KEYPAD, command->arg, MSG_RD_STS_ID_INV_ARG, 0)) return 1;
      break;
  }
  return 0;
}

/*************************************************************************
 * Function Name: updateLeds
 * Parameters: data - value to load to the PCA9502 output register
 *
 * Return: none
 *
 * Description: Loads value to the PCA9502 output register. Switch
 *              on the LED if the corresponding bit is 0, and switch
 *              off if the LED if bit is 1.
 *
 *************************************************************************/
void updateLeds(uint8_t data)
{
  /* Write to PCA9502's output register - drive LEDs */
  Data[0] = 0x0B<<3;
  Data[1] = ~data;
  I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
}

/*************************************************************************
 * Function Name: handleLedsCommand
 * Parameters: command - pointer to the command
 *
 * Return: uint32_t: 0 - command executed successfully
 *                   1 - error occured
 *
 * Description: Executes LEDs related command.
 *
 *************************************************************************/
uint32_t handleLedsCommand(ipcValueCommand_t * command)
{
  if (TASK_LEDS != command->task) return 1;
  switch(command->arg)
  {
    /* If LEDs latch should be executed */
    case SUBTASK_LEDS_LATCH:
      if (command->type != CMD_WR_ID) return 1;
      updateLeds(command->value);
      if (QS_SUCCESS != pushMessageToMaster(TASK_LEDS, SUBTASK_LEDS_LATCH, MSG_WR_STS_ID_SUCCESS, 0)) return 1;
      break;
    /* Unknown subtask */
    default:
      if (QS_SUCCESS != pushMessageToMaster(TASK_LEDS, command->arg, MSG_WR_STS_ID_FAIL, 0)) return 1;
      break;
  }
  return 0;
}

/*************************************************************************
 * Function Name: handleTimerCommand
 * Parameters: command - pointer to the command
 *
 * Return: uint32_t: 0 - command executed successfully
 *                   1 - error occured
 *
 * Description: Executes timer related command.
 *
 *************************************************************************/
uint32_t handleTimerCommand(ipcValueCommand_t * command)
{
  if (TASK_TIMER != command->task) return 1;
  switch(command->arg)
  {
    /* If new timer configuration should be executed */
    case SUBTASK_TIMER_CONFIGURE:
      if (command->type != CMD_WR_ID) return 1;
      RITIMER_Init(command->value);
      if (QS_SUCCESS != pushMessageToMaster(TASK_TIMER, SUBTASK_TIMER_CONFIGURE, MSG_WR_STS_ID_SUCCESS, 0)) return 1;
      break;
    /* If timer should be stopped */
    case SUBTASK_TIMER_STOP:
      if (command->type != CMD_WR_ID) return 1;
      RITIMER_Stop();
      GPIO_CLR4 |= 1 << 1;
      if (QS_SUCCESS != pushMessageToMaster(TASK_TIMER, SUBTASK_TIMER_STOP, MSG_WR_STS_ID_SUCCESS, 0)) return 1;
      break;
    /* Unknown subtask */
    default:
      if (QS_SUCCESS != pushMessageToMaster(TASK_TIMER, command->arg, MSG_WR_STS_ID_FAIL, 0)) return 1;
      break;
  }
  return 0;
}

/*************************************************************************
 * Function Name: routeCommand
 * Parameters: command - pointer to the command
 *
 * Return: uint32_t: 0 - command routed and executed successfully
 *                   1 - error occured
 *
 * Description: Route the command to the suitable handler
 *
 *************************************************************************/
uint32_t routeCommand(ipcValueCommand_t * command)
{
  switch(command->task)
  {
    case TASK_KEYPAD:
      return handleKeypadCommand(command);

    case TASK_LEDS:
      return handleLedsCommand(command);

    case TASK_TIMER:
      return handleTimerCommand(command);
  }
  return 1;
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
  /* Init Message and Command queues for IPC communication */
  ipcSlaveInitQueues((ipcCommandControlBlock_t*)&HOST_CMD_BUFFER_start__, (ipcMessageControlBlock_t*)&HOST_MSG_BUFFER_start__);
  /* Initialize Repetitve Interrupt Timer */
  RITIMER_Init(TIMER_VALUE_1);
  /* Configure NVIC for RI Timer interrupt handling */
  M0_RITIMER_IRQInit();
  /* Configure NVIC for Cortex-M4 Core interrupt handling */
  M0_M4CORE_IRQInit();
  /* Configure the onboard LED - GPIO4.1 */
  GPIO_CLR4 = 1 << 1;
  GPIO_DIR4 |= 1 << 1;
  /* Calculate actual clock frequency and save to I2C0_PCLK, used in i2c_drv.c */
  I2C0_PCLK = 160000000;
  /* Init I2C at 400KHz */
  I2C_InitMaster(400000);
  /* Reset PCA9502 */
  Data[0] = 0x0E<<3;
  Data[1] = 0x01<<3;
  I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
  /* Configure LED pins as outputs */
  Data[0] = 0x0A<<3;
  Data[1] = 0x0F;
  I2C_MasterWrite(PCA9502_ADDR, &Data[0], 2);
  updateLeds(0);
  /* Signals master that slave startup/init ended */
  pushMessageToMaster(TASK_SYSTEM, SERVICE_SYSTEM_INIT_END, MSG_SRV_ID, 0);
  /* Main loop */
  while(1)
  {
    /* Timer tick */
    if (flTick)
    {
      flTickClear();
      /* Invert onboard LED */
      GPIO_NOT4 = 1 << 1;
    }

    /* If there is pending command on the command queue */
    if (flCommandPending)
    {
      /* Extract command from the queue */
      status_c = ipcSlavePopCommand(&command);
      /* Diabling interrupt while clearing the CommandPending flag,
         ensures that new command event cannot be missed            */
      __cortex_m4_irq_disable();
      if (!ipcQueueHasPendingCommands()) flCommandPendingClear();
      __cortex_m4_irq_enable();
      /* If there is something wrong with queue, skip the command processing */
      if (status_c != QS_SUCCESS) continue;
      /* Route incoming command to suitable handler */
      if (routeCommand(&command)) errorCounter++;
    }

  }

}
