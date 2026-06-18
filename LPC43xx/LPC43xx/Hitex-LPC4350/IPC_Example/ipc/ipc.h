/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : ipc.h
 *    Description : Inter Process Communication (IPC) Library Header
 *
 *    History :
 *    1. Date        : December 2011
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 28 $
 **************************************************************************/
#ifndef __IPC_H
#define __IPC_H

/** include files **/
#include <stdint.h>

/** definitions **/

/* Command Types */
typedef enum {
  CMD_RD_ID = 0,
  CMD_WR_ID,
  NUMBER_OF_COMMANDS
} ipcCommands_t;

/* Message Types */
typedef enum {
  MSG_SRV_ID = 0,
  MSG_RD_ID,
  MSG_RD_STS_ID_INV_ARG,
  MSG_RD_STS_ID_RES1,
  MSG_RD_STS_ID_RES2,
  MSG_WR_STS_ID_SUCCESS,
  MSG_WR_STS_ID_FAIL,
  MSG_WR_STS_ID_RES1,
  NUMBER_OF_MESSAGES
} ipcMessages_t;

/* Message */
#pragma pack(1)

typedef union {
  uint32_t messageWord;
  struct
  {
    uint16_t      task;
    ipcMessages_t type :4;
    uint16_t      arg  :12;
  };
} ipcMessage_t;

/* Message with data(value) */
typedef union {
  struct
  {
    uint32_t messageWord;
    uint32_t valueWord;
  };
  struct
  {
    uint16_t      task;
    ipcMessages_t type :4;
    uint16_t      arg  :12;
    uint32_t      value;
  };
} ipcValueMessage_t;

/* Command */
typedef union {
  uint32_t commandWord;
  struct
  {
    uint16_t      task;
    ipcCommands_t type :4;
    uint16_t      arg  :12;
  };
} ipcCommand_t;

/* Command with data(value) */
typedef union {
  struct
  {
    uint32_t commandWord;
    uint32_t valueWord;
  };
  struct
  {
    uint16_t      task;
    ipcCommands_t type :4;
    uint16_t      arg  :12;
    uint32_t      value;
  };
} ipcValueCommand_t;

#pragma pack()

/* QUEUE STATUS */
typedef enum {

  QS_EMPTY = 0,
  QS_FULL,
  QS_SUCCESS,
  QS_ERROR

} ipcQueueStatus_t;

/* MESSAGE CONTROL BLOCK STRUCTURE */
typedef struct {
  ipcMessage_t * startAddress;
  ipcMessage_t * endAddress;
  ipcMessage_t * readPtr;
  ipcMessage_t * writePtr;
} ipcMessageControlBlock_t;

/* COMMAND CONTROL BLOCK STRUCTURE */
typedef struct {
  ipcCommand_t * startAddress;
  ipcCommand_t * endAddress;
  ipcCommand_t * readPtr;
  ipcCommand_t * writePtr;
} ipcCommandControlBlock_t;

/** public data **/

/** public functions **/

#if defined (__IPC_MASTER__)

/*************************************************************************
 * Function Name: ipcMasterPopMessage
 * Parameters: ipcMessage - pointer for storing the popped message
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Pop message from message buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcMasterPopMessage(ipcValueMessage_t * ipcMessage);

/*************************************************************************
 * Function Name: ipcMasterPushCommand
 * Parameters: ipcCommand - pointer to command
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Pushes one command into the command buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcMasterPushCommand(ipcValueCommand_t * ipcCommand);

/*************************************************************************
 * Function Name: ipcQueueHasPendingMessages
 * Parameters: none
 * Return: uint8_t 1 - has pending messages
 *                 0 - no pending messages
 *
 * Description: Checks message queue for pending messages
 *
 *************************************************************************/
uint8_t ipcQueueHasPendingMessages();

/*************************************************************************
 * Function Name: ipcMasterInitQueue
 * Parameters: cmd_cb - address of command control block,
 *             cmd_buf - address of command buffer,
 *             cmd_size - command buffer length,
 *             msg_cb - address of message control block,
 *             msg_buf - address of message buffer,
 *             msg_size - message buffer length
 *
 * Return: none
 *
 * Description: Master initialization of command and message queues
 *
 *************************************************************************/
void ipcMasterInitQueues(ipcCommandControlBlock_t * cmd_cb, ipcCommand_t * cmd_buf, uint32_t cmd_size, ipcMessageControlBlock_t * msg_cb, ipcMessage_t * msg_buf, uint32_t msg_size);

/*************************************************************************
 * Function Name: ipcNotifySlave
 * Parameters: none
 *
 * Return: none
 *
 * Description: Notifies slave that there is pending command on the queue
 *
 *************************************************************************/
void ipcNotifySlave();

#elif defined (__IPC_SLAVE__)

/*************************************************************************
 * Function Name: ipcSlavePushMessage
 * Parameters: ipcMessage - pointer to the message
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Push message to the message buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcSlavePushMessage(ipcValueMessage_t * ipcMessage);

/*************************************************************************
 * Function Name: ipcSlavePopCommand
 * Parameters: ipcCommand - pointer for storing the command
 * Return: ipcQueueStatus_t: Queue status
 *
 * Description: Pops one command from the command buffer
 *
 *************************************************************************/
ipcQueueStatus_t ipcSlavePopCommand(ipcValueCommand_t * ipcCommand);

/*************************************************************************
 * Function Name: ipcQueueHasPendingCommands
 * Parameters: none
 * Return: 1 - has pending commands
 *         0 - no pending commands
 *
 * Description: Checks command queue for pending commands
 *
 *************************************************************************/
uint8_t ipcQueueHasPendingCommands();

/*************************************************************************
 * Function Name: ipcSlaveInitQueues
 * Parameters: cmd_cb - address of command control block,
 *             msg_cb - address of message control block,
 *
 * Return: none
 *
 * Description: Slave initialization of command amd message queues
 *
 *************************************************************************/
void ipcSlaveInitQueues(ipcCommandControlBlock_t * cmd_cb, ipcMessageControlBlock_t * msg_cb);

/*************************************************************************
 * Function Name: ipcNotifyMaster
 * Parameters: none
 *
 * Return: none
 *
 * Description: Notifies master that there is pending message on the queue
 *
 *************************************************************************/
void ipcNotifyMaster();

#endif

#endif
